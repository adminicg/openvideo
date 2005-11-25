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
/** The header file for the TestSrc class.
  *
  * @author Denis Kalkofen
  *
  * $Id$
  * @file                                                                   */
 /* ======================================================================= */
#ifndef _TESTSRC_H
#define _TESTSRC_H

#include "openVideo.h"
#include "core/Node.h"

/**
*@ingroup nodes
*	TestSrc implements an 320x240 image with a white background where a red,blue and green square moves 
*	on. This node is used to test OpenVideo's environment with any input stream comming from a camera. 
*/

namespace openvideo {

class OPENVIDEO_API TestSrc :
	public Node
{
public:
	/**
	*	constructor.
	*	creates a new context and allocates memory for the image.
	*/
	TestSrc();

	/**
	*
	*/
	virtual ~TestSrc();

	/**
	*	clears the image and sets the widht and height onto its context.
	*/
	virtual void start();

	/**
	*	updates the image by moving the three squares by one pixel.
	*/
	virtual void process( );
	
	virtual void initPixelFormats();

 protected:
 	/**
	*	image 
	*/
	unsigned char *img;

	/**
	*	image widht and height
	*/
	int width,height;

	/**
	*	current square position. 
	*/
	int posX,posY;
};

} //namespace openvideo {

#endif
