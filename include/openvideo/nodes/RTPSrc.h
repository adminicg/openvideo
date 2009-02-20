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
/** The header file for the RTPSrc class.
  *
  * @author Alessandro Mulloni
  *
  * $Id: RTPSrc.h 237 2007-05-02 15:45:07Z bornik $
  * @file                                                                   */
 /* ======================================================================= */
#ifndef _RTPSRC_H
#define _RTPSRC_H

#include <openvideo/openVideo.h>

#ifdef ENABLE_RTPSRC

#include <openvideo/Node.h>

#include <liveMedia.hh>
#include <BasicUsageEnvironment.hh>
#include <GroupsockHelper.hh>

#include <winsock2.h>
#include <windows.h>


#ifdef _DEBUG
# pragma comment(lib,"live555d.lib")
#else
# pragma comment(lib,"live555.lib")
#endif

namespace openvideo {
/**
*@ingroup nodes
*	TestSrc implements an 320x240 image with a white background where a red,blue and green square moves 
*	on. This node is used to test OpenVideo's environment with any input stream comming from a camera. 
*/
class OPENVIDEO_API RTPSrc :
	public openvideo::Node
{
public:
	/**
	*	constructor.
	*	creates a new context and allocates memory for the image.
	*/
	RTPSrc();

	/**
	*
	*/
	virtual ~RTPSrc();

	/**
	*	clears the image and sets the widht and height onto its context.
	*/
	virtual void init();

	bool connect();
	bool disconnect();

	/**
	*	updates the image by moving the three squares by one pixel.
	*/
	virtual void process( );
	
	virtual void initPixelFormats();

    virtual bool setParameter(std::string key, std::string value);

	enum STATUS { STATUS_CONNECTED, STATUS_CONNECTING, STATUS_DISCONNECTED };
	STATUS getStatus() { return currentStatus; }
	void setStatus(STATUS s);

 protected:
	/**
	*	image width and height
	*/
	int width,height;

    std::string serverUrl;
	unsigned int timeout;		// NOTE: in MILLIseconds!
	unsigned int reconnectRate;	// NOTE: in MILLIseconds (time before a new reconnect trial)!

	/**
	*	current square position. 
	*/
	int posX,posY, packetReorderTime;
	unsigned int updateCtr;

	unsigned char *videoBuffer, *depthBuffer;
	unsigned int videoWidth, videoHeight, videoComp;
	HANDLE videoMutex, depthMutex;

	unsigned char *trackingBuffer;
	unsigned int trackingSize;
	HANDLE trackingMutex;

	MediaSubsession *subsessionVideo;
	MediaSubsession *subsessionTracking;

	TaskScheduler* scheduler;
	BasicUsageEnvironment *env;
	RTSPClient *client;
	MediaSession *session;

	unsigned int lastFrame;
	unsigned int lastConnectTry;

	STATUS currentStatus;
	HANDLE currentStatusMutex;

	bool stateInitialized;

	HANDLE hThread;
};

} //namespace openvideo {
#endif //#ifdef ENABLE_RTPSRC
#endif
