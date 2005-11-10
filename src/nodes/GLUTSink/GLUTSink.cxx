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
/** The source file for the GLUTSink class.
  *
  * @author Denis Kalkofen
  * 
  * $Id$
  * @file                                                                   
 /* ======================================================================= */

#include "GLUTSink.h"

#ifdef ENABLE_GLUTSINK

#include "core/Context.h"
#include "core/Manager.h"
#include <ace/Thread.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/glu.h>		   


#ifdef WIN32
#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glu32.lib")
#pragma comment(lib,"glut32.lib")
#else //linux
#endif

bool GLUTSink::isGlutThread=false;
std::vector<GLUTSink*> GLUTSink::glutSinks;
bool GLUTSink::glutRedraw=false;
ACE_Mutex* GLUTSink::redrawLock=NULL;

GLUTSink::GLUTSink()
{
	originX=originY=0;
	width=height=0;
	updateVideo=false;
	updateLock=new ACE_Token();
	if(redrawLock==NULL){
	    GLUTSink::redrawLock=new ACE_Mutex();
	}

#ifdef WIN32
	format=GL_BGR_EXT;
#else
	format=GL_RGB;
#endif
}
	
GLUTSink::~GLUTSink()
{
	delete updateLock;
	if(GLUTSink::redrawLock){
		delete GLUTSink::redrawLock;
		GLUTSink::redrawLock=NULL;
	}
}


void*
GLUTSink::mainLoop(void *)
{
    glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE );
    for(int i=0;i<(int)GLUTSink::glutSinks.size();i++)
    {
	GLUTSink *glutSink=GLUTSink::glutSinks[i];
	glutInitWindowPosition (glutSink->originX, glutSink->originY );
	int w,h;
	if(glutSink->width==0)
	    w=1;
	else
	    w=glutSink->width;
	if( glutSink->height==0)
	    h=1;
	else
	    h=glutSink->height;
	glutInitWindowSize (w,h);
	glutSink->winHandle=glutCreateWindow(glutSink->getName());
	glutDisplayFunc(GLUTSink::mainDisplayFunc);
	glutIdleFunc(GLUTSink::idleFunc);
	//////////////////////////////////////////////
	//create texture
	glEnable(GL_TEXTURE_2D);
	long data_size = 4 * sizeof(GLubyte) * TEXTURE_WIDTH * TEXTURE_HEIGHT;
	GLubyte *data = (GLubyte*)malloc(data_size);
	memset(data, 0xFF, data_size);
	glGenTextures(1, &glutSink->video_texture_id);
	glBindTexture(GL_TEXTURE_2D, glutSink->video_texture_id);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, TEXTURE_WIDTH, TEXTURE_HEIGHT, 0,
		     glutSink->format, GL_UNSIGNED_BYTE, data);
	glDisable(GL_TEXTURE_2D);
	free(data);
	//////////////////////////////////////////////
    }		
    glutMainLoop();
    
    return 0;
}

void
GLUTSink::start()
{
    printf("OV: GLUTSink -> start\n");
    //context this sink lives in
    context=this->inputs[0]->getContext();
    if(context)
    {
	this->width=context->width;
	this->height=context->height;
	if(context->format!=0)
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
    GLUTSink::glutSinks.push_back(this);
	
	if(!GLUTSink::isGlutThread){
		//start glut in a new thread
		GLUTSink::isGlutThread=true;
		ACE_hthread_t* threadHandle = new ACE_hthread_t();
		if(ACE_Thread::spawn((ACE_THR_FUNC)GLUTSink::mainLoop,
			     0, 	
			     THR_NEW_LWP|THR_JOINABLE, 	
			     0, 	
			     threadHandle,
			     0, 	
			     0, 	
			     0
	       )==-1)
		{ 
		    printf("Error in spawning thread\n"); 
		}
		return ;
    }
}

void
GLUTSink::process()
{
	if(context && context->frame){
		updateLock->acquire();
		updateVideo=true; //set flag to indicate a redraw
		updateLock->release();

		GLUTSink::redrawLock->acquire();
		GLUTSink::glutRedraw=true;//idle func calls glutPostRedisplay();
		GLUTSink::redrawLock->release();
	
		updateLock->acquire();
		while(updateVideo){//wait untill the main loop starts to update and acquires the token 'updateLock'
			updateLock->release();
			updateLock->acquire();
		} 
		updateLock->release();
	}
}


void 
GLUTSink::updateTexture()
{	

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, video_texture_id);
	
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width,
		    height, format, GL_UNSIGNED_BYTE,
		    (void*)context->frame);
    glDisable(GL_TEXTURE_2D);
	
}

void 
GLUTSink::redraw()
{
    //preGLCalls()
    glPushMatrix();
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);
    glDisable(GL_LIGHTING);
    glDisable(GL_ALPHA_TEST);
    glDisable(GL_BLEND);
    ///////////////////////    
    
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glEnable(GL_TEXTURE_2D);
    
    // redraw texture
    glBindTexture(GL_TEXTURE_2D, video_texture_id);
    glBegin(GL_QUADS);
    // video backplate
    glTexCoord2f(t_u0,t_v0); glVertex3f(-1.0f, -1.0f,  0.0f);
    glTexCoord2f(t_u1,t_v0); glVertex3f( 1.0f, -1.0f,  0.0f);
    glTexCoord2f(t_u1,t_v1); glVertex3f( 1.0f,  1.0f,  0.0f);
    glTexCoord2f(t_u0,t_v1); glVertex3f(-1.0f,  1.0f,  0.0f);
    glEnd();
    
    glDisable(GL_TEXTURE_2D);
	
    ///postGLCalls()
    glPopAttrib();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

void 
GLUTSink::idleFunc ()
{
    GLUTSink::redrawLock->acquire();
    if(GLUTSink::glutRedraw)
    {
		GLUTSink::glutRedraw=false;
		glutPostRedisplay();
    }
    GLUTSink::redrawLock->release();
   
}

void 
GLUTSink::mainDisplayFunc ()
{
    int size=(int)GLUTSink::glutSinks.size();
    for (int i=0;i<size;i++)
    {	
		glutSinks[i]->updateLock->acquire();
		if(glutSinks[i]->updateVideo)
		{	
			glutSinks[i]->updateVideo=false;
			glutSetWindow(glutSinks[i]->winHandle);
			glutSinks[i]->updateTexture();
			glutSinks[i]->redraw();
		}
		glutSinks[i]->updateLock->release();
    }
	
    glutSwapBuffers();
	
}

#endif //ENABLE_GLUTSINK
