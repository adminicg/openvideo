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
: subsession(NULL), client(NULL), session(NULL)
{
	name = typeName = "RTPSrc";
    state=new RTPSrcState();
	bufferWidth = bufferHeight = 0;
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

	// Begin by setting up our usage environment:
	logPrintS("Initializing Live555\n");
	TaskScheduler* scheduler = BasicTaskScheduler::createNew();
	if (scheduler == NULL)
	{
		logPrintE("Could not create the Live555 scheduler object\n");
		return;
	}

	env = BasicUsageEnvironment::createNew(*scheduler);
	if (env == NULL)
	{
		logPrintE("Could not create the Live555 UsageEnvironment object\n");
		return;
	}

	gettimeofday(&lastConnectTry, NULL);
	if (!connect())
	{
		logPrintE("Could not initialize Live555 RTSP video\n");
		return;
	}

	CreateThread(NULL, 0, ServerThread, env, 0, NULL);

	state->clear();

	state->width=bufferWidth;
	state->height=bufferHeight;
	state->format=PIXEL_FORMAT(FORMAT_R8G8B8);

	// make a double buffered state
	for(int i=0; i<2; i++)
	{
		unsigned char *pixels = new unsigned char[bufferWidth * bufferHeight * 3];
		memset(pixels, 255, bufferWidth * bufferHeight * 3);

		reinterpret_cast<RTPSrcState*>(state)->getBuffers().push_back(new RTPSrcBuffer(pixels, state));
	}

	posX=posY=0;
}



bool
RTPSrc::connect()
{
	client = RTSPClient::createNew(*env, 0, "OpenVideo", 0);
	if (client == NULL)
	{
		logPrintE("Could not create the Live555 RTSP client object\n");
		return false;
	}

	const char *serverURL = serverUrl.c_str();
	char* SDP = client->describeURL(serverURL);
	if (SDP) logPrintS("Successfully connected to RTSP stream %s\n", serverUrl.c_str());
	else
	{
		Medium::close(client);
		client = NULL;

		logPrintE("Could not create the Live555 SDP client object");
		return false;
	}

	session = MediaSession::createNew(*env, SDP);
	if (session == NULL)
	{
		logPrintE("Could not create the Live555 media session");
		return false;
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
			subsession->rtpSource()->setPacketReorderingThresholdTime(100000);

			// sets up the client connection
			client->setupMediaSubsession(*subsession);

			client->playMediaSession(*session);

			logPrintS("RTSP stream setup, ready to receive data...\n");

			bufferWidth = subsession->videoWidth();
			bufferHeight = subsession->videoHeight();
			if (bufferWidth == 0 || bufferHeight == 0)
			{
				logPrintE("SDP description returned zero-sized video!!!\n");
				return false;
			}

			logPrintS("RTSP video stream of size %dx%d (R8G8B8)\n", bufferWidth, bufferHeight);
			buffer = new unsigned char[bufferWidth * bufferHeight * 3];
			bufferMutex = CreateMutex(NULL, FALSE, NULL);
			if (buffer == NULL || bufferMutex == NULL) return false;

			MemorySink* memSink = MemorySink::createNew(*env, buffer, bufferWidth, bufferHeight, bufferMutex);
			subsession->sink = memSink;

			subsession->sink->startPlaying(*(subsession->readSource()), NULL, subsession);

			break;
		}
	}

	if (subsession == NULL)
	{
		logPrintE("Could not find a valid JPEG video session!!!\n");
		return false;
	}

	gettimeofday(&lastFrame, NULL);

	return true;
}



void 
RTPSrc::process()
{
	if (!buffer || !subsession) return;

	MemorySink *sink = (MemorySink*)subsession->sink;

	if (!sink || !sink->hasNewFrame())
	{
		timeval now;
		gettimeofday(&now, NULL);

		if (!sink || (now.tv_sec - lastFrame.tv_sec > 5))
		{
			if (now.tv_sec != lastConnectTry.tv_sec)
			{
				lastConnectTry = now;
				connect();
			}
		}
	}
	else
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

		sink->Lock();
		memcpy(img, buffer, bufferWidth*bufferHeight*3);
		sink->Release();

		gettimeofday(&lastFrame, NULL);

		srcBuffer->incUpdateCounter();
	}
}

bool 
RTPSrc::setParameter(std::string key, std::string value)
{
    if(Node::setParameter(key,value))
        return true;

    if(key=="serverUrl")
    {
        serverUrl = value.c_str();
        return true;
    }
	else if (key=="timeout")
	{
		timeout = atoi(value.c_str());
		return true;
	}

    return false;
}

}  // namespace openvideo


#endif //#ifdef ENABLE_RTPSRC
