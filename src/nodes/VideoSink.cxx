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
/** The source file for the VideoSink class.
  *
  * @author Denis Kalkofen
  * 
  * $Id: VideoSink.cxx 31 2005-12-10 15:42:59Z denis $
  * @file                                                                   
 /* ======================================================================= */
#include <openvideo/nodes/VideoSink.h>
#include <openvideo/openVideo.h>
#include <openvideo/nodes/VideoSinkSubscriber.h>
#ifdef  ENABLE_VIDEOSINK

using namespace openvideo;

#include <openvideo/State.h>
#include <openvideo/Manager.h>


VideoSink::VideoSink()
{
	isStarted=false;
	internalFormat=0;
	size_subscribers=0;
}

void 
VideoSink::initPixelFormats()
{
	//format_r8g8b8	= 0,
	//format_b8g8r8	= 1,
	//format_r8g8b8x8	= 2,
	//format_b8g8r8x8	= 3,
	//format_l8		= 5,
	this->pixelFormats.push_back(PIXEL_FORMAT(FORMAT_R8G8B8));
	this->pixelFormats.push_back(PIXEL_FORMAT(FORMAT_B8G8R8));
	this->pixelFormats.push_back(PIXEL_FORMAT(FORMAT_R8G8B8X8));
	this->pixelFormats.push_back(PIXEL_FORMAT(FORMAT_B8G8R8X8));
	this->pixelFormats.push_back(PIXEL_FORMAT(FORMAT_L8));
}

VideoSink::~VideoSink()
{
	
}


void
VideoSink::init()
{
	//
    //////////////////////////////////////////////
    isStarted=true;
	state=inputs[0]->getState();
}

void
VideoSink::process()
{
	if(state->frame)
	{
		//update subsrcibers
		for(int i=0;i<size_subscribers;i++)
			subsrcibers[i]->update(state);
	}	
}

void
VideoSink::postProcess()
{
	for(int i=0;i<size_subscribers;i++)
	{
		if(subsrcibers[i]->isResourceInUse())
		{
			//wait till 
		}
	}
}

void 
VideoSink::subscribe(VideoSinkSubscriber* aSubscriber)
{
	subsrcibers.push_back(aSubscriber);
	size_subscribers++;
}

#endif  //ENABLE_VideoSink
