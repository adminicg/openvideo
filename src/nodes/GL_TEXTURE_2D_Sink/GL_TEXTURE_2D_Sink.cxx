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
/** The source file for the GL_TEXTURE_2D_Sink class.
  *
  * @author Denis Kalkofen
  * 
  * $Id$
  * @file                                                                   
 /* ======================================================================= */
#include "GL_TEXTURE_2D_Sink.h"
#include "ace/Mutex.h"
#include "core/State.h"
#include "core/Manager.h"
#include <GL/gl.h>			
#include <GL/glu.h>		    

#ifdef WIN32
#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glu32.lib")
#else
#endif


using namespace openvideo;


GL_TEXTURE_2D_Sink::GL_TEXTURE_2D_Sink()
{
	mutex = new ACE_Mutex();
	width=height=0;	
	isStarted=false;
	internalFormat=0;
}

void 
GL_TEXTURE_2D_Sink::initPixelFormats()
{
	this->pixelFormats.push_back(PIXEL_FORMAT(FORMAT_R8G8B8));
	this->pixelFormats.push_back(PIXEL_FORMAT(FORMAT_B8G8R8X8));
}

GL_TEXTURE_2D_Sink::~GL_TEXTURE_2D_Sink()
{
	delete mutex;
}

/**
*	aquires the mutex	
*/   
void 
GL_TEXTURE_2D_Sink::acquire()
{
	mutex->acquire();
}


/**
*	releases the mutex.	
*/   
void 
GL_TEXTURE_2D_Sink::release()
{
	mutex->release();
}

void
GL_TEXTURE_2D_Sink::start()
{
	mutex->acquire();

	
	/* glTexImage2D supported formats
	/* original gl spec. 
	GL_COLOR_INDEX 	Each element is a single value, a color index. It is converted to fixed point (with an unspecified number of 0 bits to the right of the binary point), shifted left or right depending on the value and sign of GL_INDEX_SHIFT, and added to GL_INDEX_OFFSET (see glPixelTransfer). The resulting index is converted to a set of color components using the GL_PIXEL_MAP_I_TO_R, GL_PIXEL_MAP_I_TO_G, GL_PIXEL_MAP_I_TO_B, and GL_PIXEL_MAP_I_TO_A tables, and clamped to the range [0,1].
	GL_RED 	Each element is a single red component. It is converted to floating point and assembled into an RGBA element by attaching 0.0 for green and blue, and 1.0 for alpha. Each component is then multiplied by the signed scale factor GL_c_SCALE, added to the signed bias GL_c_BIAS, and clamped to the range [0,1] (see glPixelTransfer).
	GL_GREEN 	Each element is a single green component. It is converted to floating point and assembled into an RGBA element by attaching 0.0 for red and blue, and 1.0 for alpha. Each component is then multiplied by the signed scale factor GL_c_SCALE, added to the signed bias GL_c_BIAS, and clamped to the range [0,1] (see glPixelTransfer).
	GL_BLUE 	Each element is a single blue component. It is converted to floating point and assembled into an RGBA element by attaching 0.0 for red and green, and 1.0 for alpha. Each component is then multiplied by the signed scale factor GL_c_SCALE, added to the signed bias GL_c_BIAS, and clamped to the range [0,1] (see glPixelTransfer).
	GL_ALPHA 	Each element is a single red component. It is converted to floating point and assembled into an RGBA element by attaching 0.0 for red, green, and blue. Each component is then multiplied by the signed scale factor GL_c_SCALE, added to the signed bias GL_c_BIAS, and clamped to the range [0,1] (see glPixelTransfer).
	GL_RGB 	Each element is an RGB triple. It is converted to floating point and assembled into an RGBA element by attaching 1.0 for alpha. Each component is then multiplied by the signed scale factor GL_c_SCALE, added to the signed bias GL_c_BIAS, and clamped to the range [0,1] (see glPixelTransfer).
	GL_RGBA 	Each element is a complete RGBA element. It is converted to floating point. Each component is then multiplied by the signed scale factor GL_c_SCALE, added to the signed bias GL_c_BIAS, and clamped to the range [0,1] (see glPixelTransfer).
	GL_LUMINANCE 	Each element is a single luminance value. It is converted to floating point, and then assembled into an RGBA element by replicating the luminance value three times for red, green, and blue, and attaching 1.0 for alpha. Each component is then multiplied by the signed scale factor GL_c_SCALE, added to the signed bias GL_c_BIAS, and clamped to the range [0,1] (see glPixelTransfer).
	GL_LUMINANCE_ALPHA 	Each element is a luminance/alpha pair. It is converted to floating point, and then assembled into an RGBA element by replicating the luminance value three times for red, green, and blue. Each component is then multiplied by the signed scale factor GL_c_SCALE, added to the signed bias GL_c_BIAS, and clamped to the range [0,1] (see glPixelTransfer).

	// windows extension
	GL_BGR_EXT 	Each pixel is a group of three components in this order: blue, green, red. GL_BGR_EXT provides a format that matches the memory layout of Windows device-independent bitmaps (DIBs). Thus your applications can use the same data with Win32 function calls and OpenGL pixel function calls.
	GL_BGRA_EXT 	Each pixel is a group of four components in this order: blue, green, red, alpha. GL_BGRA_EXT provides a format that matches the memory layout of Windows device-independent bitmaps (DIBs). Thus your applications can use the same data with Win32 function calls and OpenGL pixel function calls.

	*/
	/*
enum PIXEL_FORMAT {
	FORMAT_R8G8B8	= 0,
	FORMAT_B8G8R8	= 1,
	FORMAT_R8G8B8X8	= 2,
	FORMAT_B8G8R8X8	= 3,
	FORMAT_R5G6B5	= 4,
	FORMAT_L8		= 5,

	FORMAT_UNKNOWN	= 6
};
	*/
	// map ov to gl pixel format and set gl_texture_2d_sink's format
	switch(curPixelFormat)
	{
		case FORMAT_R8G8B8:
			format=GL_RGB;
			internalFormat=3;
			break;

		case FORMAT_B8G8R8:
//			format=;
//			internalFormat
			break;
		
		case FORMAT_R8G8B8X8:
//			format=;
			break;

		case FORMAT_B8G8R8X8:
			format=GL_BGR_EXT;
			break;


		case FORMAT_R5G6B5:
//			format=;
			break;

		case FORMAT_L8:
//			format=;
			break;

		default:
			printf("GL_TEXTURE_2D_Sink does not suppport the current pixel format %s\n",
				(PixelFormat::FormatToString(curPixelFormat)).c_str());
			exit(-1);

    }
	//
    printf("OVpenVideo: start GL_TEXTURE_2D_Sink '%s' \n",name.c_str());
    state=this->inputs[0]->getState();
    if(state)
    {
		this->width=state->width;
		this->height=state->height;
	}
    flip_h = false; 
    flip_v = true;
    float u_rt = (float)width / TEXTURE_WIDTH;
    float v_rt = (float)height / TEXTURE_HEIGHT;
    t_u0 = (flip_h ? u_rt : 0   );
    t_u1 = (flip_h ?    0 : u_rt);
    t_v0 = (flip_v ? v_rt : 0   );
    t_v1 = (flip_v ?    0 : v_rt);
    //
    //create texture
    glEnable(GL_TEXTURE_2D);
    long data_size = 4 * sizeof(GLubyte) * TEXTURE_WIDTH * TEXTURE_HEIGHT;
    GLubyte *data = (GLubyte*)malloc(data_size);
    memset(data, 0xFF, data_size);
    glGenTextures(1, &video_texture_id);
    glBindTexture(GL_TEXTURE_2D, video_texture_id);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, TEXTURE_WIDTH, TEXTURE_HEIGHT, 0,
		 GL_BGR_EXT, GL_UNSIGNED_BYTE, data);
    glDisable(GL_TEXTURE_2D);
    free(data);
    //////////////////////////////////////////////
    isStarted=true;
	mutex->release();
}



void
GL_TEXTURE_2D_Sink::process()
{
	mutex->acquire();
	if(state->frame)
	{
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, video_texture_id);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width,
						height, format, GL_UNSIGNED_BYTE,
						(void*)state->frame);

		glDisable(GL_TEXTURE_2D);
	}
	mutex->release();
}
