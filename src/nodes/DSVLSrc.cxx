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
/** The source file for the DSVLSrc class.
  *
  * @author Denis Kalkofen
  * 
  * $Id: DSVLSrc.cxx 30 2005-12-10 12:10:50Z denis $
  * @file                                                                   
 /* ======================================================================= */

#include <openvideo/nodes/DSVLSrc.h>
#include <openvideo/openVideo.h>

#ifdef ENABLE_DSVLSRC

#include <DSVL.h>
#include <openvideo/Manager.h>

#ifdef OV_IS_WINXP
#  ifdef OV_IS_DEBUG
#    pragma message (">>> Linking against debug build of DSVL")
#    pragma comment(lib,"DSVLd.lib")
#  else	
#    pragma message (">>> Linking against release build of DSVL")
#    pragma comment(lib,"DSVL.lib")
#  endif
#else //WIN32

#endif

#include <openvideo/State.h>
#include <assert.h>

using namespace openvideo;

DSVLSrc::DSVLSrc()
{
	mbHandle = new MemoryBufferHandle;	
	dsvlSource = NULL;

	name = "DSVLSrc";
	flipV = false;
	frameBuffer = NULL;
}
	
DSVLSrc::~DSVLSrc()
{
	dsvlSource->CheckinMemoryBuffer(*mbHandle);

	delete dsvlSource;
	dsvlSource = NULL;

	delete mbHandle;
	mbHandle = NULL;
}

void 
DSVLSrc::initPixelFormats()
{
	pixelFormats.push_back(PIXEL_FORMAT(FORMAT_B8G8R8));
}


void
DSVLSrc::init()
{
	Manager::getInstance()->getLogger()->log("OpenVideo: start DSVLSrc\n");

	if(::CoInitialize(NULL) == S_FALSE)
	{
		Manager::getInstance()->getLogger()->log("OpenVideo: ERROR - CoInitialize() failed\n");
		return;
	}

	dsvlSource = new DSVL_VideoSource();

	LONG	cap_width = 0;
	LONG	cap_height = 0;
	double	cap_fps = 0.0;

	if(FAILED(dsvlSource->BuildGraphFromXMLFile(const_cast<char*>(configFileName.c_str()))))
	{
		Manager::getInstance()->getLogger()->log("OpenVideo: ERROR - DSVL failed reading config file\n");
		return;
	}

	if(FAILED(dsvlSource->GetCurrentMediaFormat(&cap_width,&cap_height,&cap_fps,NULL)))
	{
		Manager::getInstance()->getLogger()->log("OpenVideo: ERROR - DSVL failed retrieving video configuration\n");
		return;
	}

	if(FAILED(dsvlSource->EnableMemoryBuffer(2)))
	{
		Manager::getInstance()->getLogger()->log("OpenVideo: ERROR - DSVL setting memory buffers\n");
		return;
	}

	if(FAILED(dsvlSource->Run()))
	{
		Manager::getInstance()->getLogger()->log("OpenVideo: ERROR - DSVL failed to run\n");
		return;
	}

	Manager::getInstance()->getLogger()->logEx("OpenVideo: DSVL initialized with %d x %d at %d fps\n", cap_width, cap_height, (int)cap_fps);

	state=new State();
	state->clear();
	state->width=cap_width;
	state->height=cap_height;
	state->format = FORMAT_B8G8R8;
	state->frame = NULL;

	if(flipV)
	{
		assert(state->format == FORMAT_B8G8R8);
		frameBuffer = new unsigned char[state->width*state->height*3];
	}
}


void
DSVLSrc::preProcess()
{
}


static void
flipImage(unsigned char* nSrc, unsigned char *nDst, int nStride, int nHeight)
{
	nSrc += nStride*(nHeight-1);

	for(int i=0; i<nHeight; i++)
	{
		memcpy(nDst, nSrc, nStride);
		nDst += nStride;
		nSrc -= nStride;
	}
}

void
DSVLSrc::process()
{
	DWORD wait_result = dsvlSource->WaitForNextSample(100/60);

	if(flipV)
	{
		unsigned char* tmpPtr = NULL;
		if(!SUCCEEDED(dsvlSource->CheckoutMemoryBuffer(mbHandle, &tmpPtr)))
		{
			Manager::getInstance()->getLogger()->log("OpenVideo: ERROR - DSVL failed to deliver frame\n");
			return;
		}

		flipImage(tmpPtr, frameBuffer, state->width*3, state->height);
		state->frame = frameBuffer;
		dsvlSource->CheckinMemoryBuffer(*mbHandle);
	}
	else
	{
		if(!SUCCEEDED(dsvlSource->CheckoutMemoryBuffer(mbHandle, &state->frame)))
		{
			Manager::getInstance()->getLogger()->log("OpenVideo: ERROR - DSVL failed to deliver frame\n");
			return;
		}
	}



	//FILE* fp = fopen("dump.raw", "wb");
	//fwrite(state->frame, 1, state->width*state->height*3, fp);
	//fclose(fp);
}

void
DSVLSrc::postProcess()
{
	if(!flipV)
		dsvlSource->CheckinMemoryBuffer(*mbHandle);
	state->frame = NULL;
}


bool 
DSVLSrc::setParameter(std::string key, std::string value)
{
	if(Node::setParameter(key,value))
		return true;

	if(key=="config-file")
	{
		configFileName = value;
		return true;
	}

	if(key=="flip-v")
	{
		flipV = (value=="true" || value=="TRUE");
		return true;
	}

	return false;
}


#endif //ENABLE_DSVLSRC
