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
/** The source file for the VideoWrapperSrc class.
  *
  * @author Denis Kalkofen
  * 
  * $Id: 
  * @file                                                                   
 /* ======================================================================= */

#include "VideoWrapperSrc.h"

#ifdef WIN32
	#ifdef _DEBUG
		#pragma comment(lib,"VideoWrapperd.lib")
	#else	
		#pragma comment(lib,"VideoWrapper.lib")
	#endif
#else //WIN32

#endif

#include "core/Context.h"

VideoWrapperSrc::VideoWrapperSrc()
{
	strcpy(libId,"");
	strcpy(formatId,"");
	cameraNum=width=height=frameRate=format=0;
	scale=0.0;
}
	
VideoWrapperSrc::~VideoWrapperSrc()
{
}

void
VideoWrapperSrc::start()
{
	printf("OpenVideo: start VideoWrapperSrc\n");

	char camInitString[256];//="vc: 0 320x240 15 UYVY 0";
	sprintf(camInitString,"%s: %i %ix%i %i %s %f\n",libId,cameraNum,width,height,frameRate,formatId,scale);;
	printf("camInitString= %s",camInitString);

	if(VIDEO_openVideo(camInitString, &g_hVideo)!= VW_SUCCESS)
	{
		printf("failed to open video\n");
	}

//	VIDEO_setPropertyLong(g_hVideo,CAMERAPROP_BRIGHT,1);


	// start video
	if(VIDEO_startVideo(g_hVideo)!= VW_SUCCESS)
	{
		printf("failed to start video\n");
	}

	// get video size
	VIDEO_getWidth(g_hVideo, &width);
	VIDEO_getHeight(g_hVideo, &height);

	// get pixel format
	VIDEO_getPixelFormat(g_hVideo, &format);

	// display video properties
	//printf("video is %d x %d, and format is %d \n", width,height, format);
	
	context=new Context();
	context->clear();
	context->width=width;
	context->height=height;
	context->format=format;	
}

void
VideoWrapperSrc::process()
{
	timeval timestamp;

	context->frame=NULL;
	VIDEO_getFrame(g_hVideo, &(context->frame), &timestamp);

}

void
VideoWrapperSrc::postProcess()
{
	VIDEO_releaseFrame(g_hVideo);
	context->frame=NULL;
}




bool 
VideoWrapperSrc::setParameter(std::string key, std::string value)
{
	if(Node::setParameter(key,value)) return true;

	if(key=="libId")
	{
		strcpy(this->libId,value.c_str());
		return true;
	}
	else if(key=="cameraNum")
	{
		this->cameraNum=atoi(value.c_str());
		return true;
	}
	else if(key=="width")
	{
		this->width=atoi(value.c_str());
		return true;
	}
	else if(key=="height")
	{
		this->height=atoi(value.c_str());
		return true;
	}
	else if(key=="frameRate")
	{
		this->frameRate=atoi(value.c_str());
		return true;
	}
	else if(key=="formatId")
	{
		strcpy(formatId,value.c_str());
		return true;
	}
	else if(key=="scale")
	{
		this->scale=(float)atof(value.c_str());
		return true;
	}

	return false;
}
