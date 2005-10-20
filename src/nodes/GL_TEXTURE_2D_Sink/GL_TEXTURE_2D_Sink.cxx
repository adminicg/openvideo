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
  * $Id: 
  * @file                                                                   
 /* ======================================================================= */
#include "GL_TEXTURE_2D_Sink.h"
#include "ace/Mutex.h"
#include "core/Context.h"
#include "core/Manager.h"
#include <GL/gl.h>			
#include <GL/glu.h>		    

#ifdef WIN32
#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glu32.lib")
#else
#endif


GL_TEXTURE_2D_Sink::GL_TEXTURE_2D_Sink()
{
	mutex = new ACE_Mutex();
	width=height=0;	
	isStarted=false;
#ifdef WIN32
	format=GL_BGR_EXT;
#else
	format=GL_RGB;
#endif
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
    printf("OV: GL_TEXTURE_2D_Sink -> start\n");
    context=this->inputs[0]->getContext();
    if(context)
    {
	this->width=context->width;
	this->height=context->height;
	if(context->format)
	    this->format=context->format;
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
		 format, GL_UNSIGNED_BYTE, data);
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
	if(context->frame)
	{
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, video_texture_id);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width,
						height, format, GL_UNSIGNED_BYTE,
						(void*)context->frame);

		glDisable(GL_TEXTURE_2D);
	}
	mutex->release();
}
