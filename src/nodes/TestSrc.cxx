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
/** The source file for the TestSrc class.
  *
  * @author Denis Kalkofen
  * 
  * $Id: TestSrc.cxx 30 2005-12-10 12:10:50Z denis $
  * @file                                                                   
 /* ======================================================================= */
#include <openvideo/nodes/TestSrc.h>
#include <openvideo/openVideo.h>
#ifdef ENABLE_TESTSRC
#include <openvideo/Manager.h>
#include <openvideo/State.h>

using namespace openvideo;

// constructor
TestSrc::TestSrc()
{
    state=new State();
    width=320;
    height=240;
    img=new unsigned char[width*height*3];
}

// destructor
TestSrc::~TestSrc()
{
    delete state;
    delete img;
}

void 
TestSrc::initPixelFormats()
{
	pixelFormats.push_back(PIXEL_FORMAT(FORMAT_R8G8B8));
	pixelFormats.push_back(PIXEL_FORMAT(FORMAT_B8G8R8));
}

void 
TestSrc::init()
{
    Manager::getInstance()->getLogger()->log("OV: TestSrc -> start\n");
    state->clear();

    state->width=width;
    state->height=height;

   
    for(int i=0;i<(width*height*3);i++)
    {
		img[i]=255;
    }
    posX=posY=0;
}


void 
TestSrc::process()
{
    unsigned char R,G,B;
    int dist=10;
    for(int y=posY;y<posY+dist;y++)
    {
	int ix=0;
	for(int x=posX;x<posX+(dist*3);x++)
	{
	    
	    R=255;
	    G=255;
	    B=255;
	    
	    img[(3*width*y)+(3*x)  ]=R;
	    img[(3*width*y)+(3*x)+1]=G;
	    img[(3*width*y)+(3*x)+2]=B;
	}
    }
    posX++;;
    if(posX>=width-(dist*3)){
	posX=0;
	posY=posY+dist;
	if(posY>=height-dist)
	    posY=0;
    }
   
   
    for(int y=posY;y<posY+dist;y++)
    {
	int ix=0;
	for(int x=posX;x<posX+(dist*3);x++)
	{
	    if(ix<=dist){
		R=255;
		G=0;
		B=0;
	    }else if(ix>dist && ix<=dist*2){
		R=0;
		G=255;
		B=0;
	    }else if(ix>dist*2 && ix<dist*3){
		R=0;
		G=0;
		B=255;
	    }else if(ix>=dist*3){
		ix=0;
	    }

	    img[(3*width*y)+(3*x)  ]=R;
	    img[(3*width*y)+(3*x)+1]=G;
	    img[(3*width*y)+(3*x)+2]=B;
	    ix++;
	}
    }
    state->frame=img;
}
#endif //#ifdef ENABLE_TESTSRC
