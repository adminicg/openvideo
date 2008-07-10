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

#include <winsock2.h>
#include <windows.h>

#include <liveMedia.hh>
#include <BasicUsageEnvironment.hh>

# pragma comment(lib,"live555d.lib")
# pragma comment(lib,"libJPEGd.lib")


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

	/**
	*	updates the image by moving the three squares by one pixel.
	*/
	virtual void process( );
	
	virtual void initPixelFormats();

    virtual bool setParameter(std::string key, std::string value);

 protected:
	/**
	*	image width and height
	*/
	int width,height;

    std::string serverUrl;

	/**
	*	current square position. 
	*/
	int posX,posY;
	unsigned int updateCtr;

	enum { bufferWidth = 320, bufferHeight = 240 };
	unsigned char *buffer;

	HANDLE bufferMutex;

	MediaSubsession *subsession;
};

} //namespace openvideo {
#endif //#ifdef ENABLE_RTPSRC
#endif
