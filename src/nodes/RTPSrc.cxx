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

DWORD WINAPI ConnectThread(LPVOID lpParam)
{
	RTPSrc* src = (RTPSrc*)lpParam;
	return src->connect();
}


// Allows TestSrc to set internal data of openvideo::Frame
class RTPSrcBuffer : public Buffer
{
friend class RTPSrc;
public:
	RTPSrcBuffer(unsigned char* pixbuffer, unsigned char *trackBuffer, State* state)
	{
		buffer = pixbuffer;
		usrData = trackBuffer;
		width = state->width;
		height = state->height;
		format = state->format;
	}

	~RTPSrcBuffer()
	{
		delete buffer;
		buffer = NULL;

		delete usrData;
		usrData = NULL;
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
	: subsessionVideo(NULL), videoBuffer(NULL), videoWidth(0), videoHeight(0), videoMutex(NULL),
	  subsessionTracking(NULL), trackingBuffer(NULL), trackingSize(0), trackingMutex(NULL),
	  client(NULL), session(NULL), currentStatus(STATUS_DISCONNECTED), stateInitialized(false),
	  hThread(NULL)
{
	name = typeName = "RTPSrc";
    state=new RTPSrcState();
    updateCtr = 1;
    packetReorderTime = 1000000;
    timeout = 5000;

	currentStatusMutex = CreateMutex(NULL, FALSE, NULL);
}

// destructor
RTPSrc::~RTPSrc()
{
    delete state;


}


void
RTPSrc::setStatus(STATUS s)
{
	WaitForSingleObject(currentStatusMutex, INFINITE);
	currentStatus = s;
	ReleaseMutex(currentStatusMutex);
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

	posX=posY=0;
}



bool
RTPSrc::connect()
{
	client = RTSPClient::createNew(*env, 0, "OpenVideo", 0);
	if (client == NULL)
	{
		logPrintE("Could not create the Live555 RTSP client object\n");
		setStatus(STATUS_DISCONNECTED);
		return false;
	}

	const char *serverURL = serverUrl.c_str();
	char* SDP = client->describeURL(serverURL);
	if (SDP) logPrintS("Successfully connected to RTSP stream %s\n", serverUrl.c_str());
	else
	{
		Medium::close(client);
		client = NULL;

		logPrintE("Could not create the Live555 SDP client object\n");
		setStatus(STATUS_DISCONNECTED);
		return false;
	}

	session = MediaSession::createNew(*env, SDP);
	if (session == NULL)
	{
		logPrintE("Could not create the Live555 media session\n");
		setStatus(STATUS_DISCONNECTED);
		return false;
	}

	MediaSubsessionIterator iter(*session);
	MediaSubsession *subsession;
	while ((subsession = iter.next()) != NULL)
	{
		if (strcmp(subsession->mediumName(), "video") == 0)
		{
			if (!subsession->initiate()) continue;
			logPrintS("Initialized receiver for %s/%s\n", subsession->mediumName(), subsession->codecName());

			if (!subsession->rtpSource()) continue;

			// one second threshold.. change this if you need less lag, but it might
			// give you more lost packets
			subsession->rtpSource()->setPacketReorderingThresholdTime(packetReorderTime);
            logPrintI("setPacketReorderingThresholdTime %i\n", packetReorderTime);
			// sets up the client connection
			client->setupMediaSubsession(*subsession);

			client->playMediaSession(*session);

			logPrintS("RTSP stream setup, ready to receive video data...\n");

			videoWidth = subsession->videoWidth();
			videoHeight = subsession->videoHeight();
			if (videoWidth == 0 || videoHeight == 0)
			{
				logPrintE("SDP description returned zero-sized video!!!\n");
				setStatus(STATUS_DISCONNECTED);
				return false;
			}

			logPrintS("RTSP video stream of size %dx%d (R8G8B8)\n", videoWidth, videoHeight);
			videoBuffer = new unsigned char[videoWidth * videoHeight * 3];
			videoMutex = CreateMutex(NULL, FALSE, NULL);
			if (videoBuffer == NULL || videoMutex == NULL) return false;

			MemorySinkJPEG* memSink = MemorySinkJPEG::createNew(*env, videoBuffer, videoWidth, videoHeight, videoMutex);
			subsession->sink = memSink;

			subsession->sink->startPlaying(*(subsession->readSource()), NULL, subsession);

			subsessionVideo = subsession;
		}
		else if (strcmp(subsession->mediumName(), "custom") == 0)
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

			logPrintS("RTSP stream setup, ready to receive tracking data...\n");

			trackingSize = 100;
			trackingBuffer = new unsigned char[trackingSize];
			trackingMutex = CreateMutex(NULL, FALSE, NULL);
			if (trackingBuffer == NULL || trackingMutex == NULL) return false;

			MemorySink* memSink = MemorySink::createNew(*env, trackingBuffer, trackingSize, trackingMutex);
			subsession->sink = memSink;

			subsession->sink->startPlaying(*(subsession->readSource()), NULL, subsession);

			subsessionTracking = subsession;
		}
	}

	if (!subsessionVideo || !subsessionTracking)
	{
		logPrintI("Could not find a valid scout server, scout seems to be offline.\n");
		setStatus(STATUS_DISCONNECTED);
		return false;
	}

	if (!stateInitialized)
	{
		state->clear();
		state->width=videoWidth;
		state->height=videoHeight;
		state->format=PIXEL_FORMAT(FORMAT_R8G8B8);

		// make a double buffered state
		for(int i=0; i<2; i++)
		{
			unsigned char *pixels = new unsigned char[videoWidth * videoHeight * 3];
			memset(pixels, 0, videoWidth * videoHeight * 3);

			unsigned char *tracking = new unsigned char[trackingSize];
			memset(tracking, 0, trackingSize);

			reinterpret_cast<RTPSrcState*>(state)->getBuffers().push_back(new RTPSrcBuffer(pixels, tracking, state));
		}

		stateInitialized = true;
	}

	lastFrame = GetTickCount();

	setStatus(STATUS_CONNECTED);

	hThread = CreateThread(NULL, 0, ServerThread, env, 0, NULL);

	return true;
}



void 
RTPSrc::process()
{
	switch (getStatus())
	{
		case STATUS_CONNECTING:
			return;

		case STATUS_DISCONNECTED:
		{
			unsigned int now = GetTickCount();

			// try to reconnect every 2 seconds..
			if (now - lastConnectTry > 2000)
			{
				lastConnectTry = now;

				setStatus(STATUS_CONNECTING);
				CreateThread(NULL, 0, ConnectThread, this, 0, NULL);
			}
		}
		break;

		case STATUS_CONNECTED:
		{
			if (subsessionVideo && videoBuffer && subsessionTracking && trackingBuffer)
			{
				MemorySinkJPEG *sinkVideo = (MemorySinkJPEG*)subsessionVideo->sink;
				MemorySink *sinkTracking = (MemorySink*)subsessionTracking->sink;

				if (!sinkVideo || !sinkVideo->hasNewFrame())
				{
					unsigned int now = GetTickCount();

					if (!sinkVideo || ((now - lastFrame) > timeout))
					{
						subsessionVideo = NULL;
						subsessionTracking = NULL;
						
						if (hThread)
						{
							TerminateThread(hThread, 0);
							hThread = NULL;
						}

						setStatus(STATUS_DISCONNECTED);
					}
				}
				else
				{
					RTPSrcBuffer* srcBuffer = reinterpret_cast<RTPSrcBuffer*>(state->findFreeBuffer());

					if(!srcBuffer)
					{
						logPrintW("RTPSrc all buffers locked, can not read a new camera image!\n");
						return;
					}

					if (sinkVideo->isCorruptedFrame())
					{
						logPrintW("RTPSrc didn't output a frame: JPEG was corrupted\n");
					}
					else
					{
						if (sinkVideo)
						{
							unsigned char* img = const_cast<unsigned char*>(srcBuffer->getPixels());

							sinkVideo->Lock();
							memcpy(img, videoBuffer, videoWidth*videoHeight*3);
							sinkVideo->Release();
						}

						if (sinkTracking)
						{
							unsigned char* track = (unsigned char*)(srcBuffer->getUserData());

							sinkTracking->Lock();
							memcpy(track, trackingBuffer, trackingSize);
							sinkTracking->Release();
						}
					}

					reinterpret_cast<RTPSrcState*>(state)->setCurrentBuffer(srcBuffer);
					srcBuffer->incUpdateCounter();
					updateCtr++;

					lastFrame = GetTickCount();
				}
			}
			break;
		}
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
		// NOTE: convert from seconds to milliseconds..
		timeout = atoi(value.c_str()) * 1000;
		return true;
	}
    else if (key=="packetReorderTime")
    {
        packetReorderTime = atoi(value.c_str());
        return true;
    }

    return false;
}

}  // namespace openvideo


#endif //#ifdef ENABLE_RTPSRC
