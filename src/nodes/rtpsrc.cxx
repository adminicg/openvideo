/* ========================================================================
 * Copyright (C) 2005  Graz University of Technology
 *
 * This framework is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This framework is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this framework; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * For further information please contact Denis Kalkofen under
 * <kalkofen@icg.tu-graz.ac.at> or write to Denis Kalkofen,
 * Graz University of Technology, Inffeldgasse 16a, A8010 Graz,
 * Austria.
 * ========================================================================
 * PROJECT: OpenVideo
 * ======================================================================== */
/** The source file for the RTPSrc class.
  *
  * @author Denis Kalkofen
  * 
  * $Id: TestSrc.cxx 237 2007-05-02 15:45:07Z bornik $
  * @file                                                                   
  * ======================================================================= */


#include <openvideo/nodes/RTPSrc.h>
#include <openvideo/openVideo.h>


#ifdef ENABLE_RTPSRC


#include <openvideo/Manager.h>
#include <openvideo/State.h>
#include <iostream>


namespace openvideo {


DWORD WINAPI ServerThread(LPVOID lpParam)
{
	UsageEnvironment* env = (UsageEnvironment*)lpParam;
	env->taskScheduler().doEventLoop();
	return 0;
}


// Allows TestSrc to set internal data of openvideo::Frame
class RTPSrcBuffer : public Buffer
{
friend class RTPSrc;
public:
	RTPSrcBuffer(unsigned char* pixbuffer, State* state)
	{
		buffer = pixbuffer;
		width = state->width;
		height = state->height;
		format = state->format;
	}

	~RTPSrcBuffer()
	{
		delete buffer;
		buffer = NULL;
	}

	//void setState(Frame::STATE newstate)  {  state = newstate;  }
	void incUpdateCounter()  {  updateCtr++;  }
};


// Allows TestSrc to set internal data of openvideo::State
class RTPSrcState : public State
{

public:
	~RTPSrcState()
	{
	  for(size_t i=0; i<buffers.size(); i++)
	    delete buffers[i];
	  buffers.clear();
	}
	BufferVector& getBuffers()  {  return buffers;  }
	void setCurrentBuffer(Buffer* buf)  {  currentBuffer = buf;  }
};



// constructor
RTPSrc::RTPSrc()
{
	name = typeName = "RTPSrc";
    state=new RTPSrcState();
    width=320;
    height=240;
    updateCtr = 1;
}

// destructor
RTPSrc::~RTPSrc()
{
    delete state;


}

void 
RTPSrc::initPixelFormats()
{
	pixelFormats.push_back(PIXEL_FORMAT(FORMAT_R8G8B8));
}

void 
RTPSrc::init()
{
    logPrintS("Building RTPSrc\n");
    state->clear();

    state->width=width;
    state->height=height;
	state->format=PIXEL_FORMAT(FORMAT_R8G8B8);

	// make a double buffered state
	for(int i=0; i<2; i++)
	{
		unsigned char *pixels = new unsigned char[width*height*3];
		memset(pixels, 255, width*height*3);

		reinterpret_cast<RTPSrcState*>(state)->getBuffers().push_back(new RTPSrcBuffer(pixels, state));
	}
	
	// Begin by setting up our usage environment:
	logPrintS("Initializing Live555\n");
	TaskScheduler* scheduler = BasicTaskScheduler::createNew();
	if (scheduler == NULL)
	{
		logPrintE("Could not create the Live555 scheduler object");
		return;
	}

	UsageEnvironment* env = BasicUsageEnvironment::createNew(*scheduler);
	if (env == NULL)
	{
		logPrintE("Could not create the Live555 UsageEnvironment object");
		return;
	}

	RTSPClient *client = RTSPClient::createNew(*env, 0, "OpenVideo", 0);
	if (client == NULL)
	{
		logPrintE("Could not create the Live555 RTSP client object");
		return;
	}

	const char *serverURL = "rtsp://localhost:8554/scout";
	char* SDP = client->describeURL(serverURL);
	if (SDP) logPrintS("Successfully connected to RTSP stream %s\n", serverURL);
	else
	{
		logPrintE("Could not create the Live555 SDP client object");
		return;
	}

	MediaSession *session = MediaSession::createNew(*env, SDP);
	if (session == NULL)
	{
		logPrintE("Could not create the Live555 media session");
		return;
	}

	MediaSubsessionIterator iter(*session);
	while ((subsession = iter.next()) != NULL)
	{
		if (strcmp(subsession->codecName(), "JPEG") == 0)
		{
			if (!subsession->initiate()) continue;
			logPrintS("Initialized receiver for %s/%s\n", subsession->mediumName(), subsession->codecName());

			if (!subsession->rtpSource()) continue;

			// one second threshold.. change this if you need less lag, but it might
			// give you more lost packets
			subsession->rtpSource()->setPacketReorderingThresholdTime(1000000);

			// sets up the client connection
			client->setupMediaSubsession(*subsession);

			client->playMediaSession(*session);
			
			logPrintS("RTSP stream setup, ready to receive data...");
			
			buffer = new unsigned char[bufferWidth * bufferHeight * 3];
			bufferMutex = CreateMutex(NULL, FALSE, NULL);

			MemorySink* memSink = MemorySink::createNew(*env, buffer, bufferWidth, bufferHeight, bufferMutex);
			subsession->sink = memSink;

			subsession->sink->startPlaying(*(subsession->readSource()), NULL, subsession);

			break;
		}
	}

	if (subsession != NULL)
		CreateThread(NULL, 0, ServerThread, env, 0, NULL);

	posX=posY=0;
}


void 
RTPSrc::process()
{
  using namespace std;
  //cerr << "errr" << endl;
    RTPSrcBuffer* srcBuffer = reinterpret_cast<RTPSrcBuffer*>(state->findFreeBuffer());

    if(!srcBuffer)
	{
		logPrintW("RTPSrc all buffers locked, can not read a new camera image!\n");
		return;
    }

    reinterpret_cast<RTPSrcState*>(state)->setCurrentBuffer(srcBuffer);
    updateCtr++;
    

    unsigned char* img = const_cast<unsigned char*>(srcBuffer->getPixels());

	WaitForSingleObject(bufferMutex, INFINITE);
	memcpy(img, buffer, bufferWidth*bufferHeight*3);
	ReleaseMutex(bufferMutex);

	srcBuffer->incUpdateCounter();
}


}  // namespace openvideo


#endif //#ifdef ENABLE_RTPSRC
