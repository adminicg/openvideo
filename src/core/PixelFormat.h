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
/** The header file for the Context class.
*
* @author Daniel Wagner
*
* $Id: 
* @file                                                                   */
/* ======================================================================= */

#ifndef _PIXELFORMAT_H
#define _PIXELFORMAT_H

#include <openVideo.h>
#include <string>


enum PIXEL_FORMAT {
	FORMAT_R8G8B8	= 0,
	FORMAT_B8G8R8	= 1,
	FORMAT_R8G8B8X8	= 2,
	FORMAT_B8G8R8X8	= 3,
	FORMAT_R5G6B5	= 4,
	FORMAT_L8		= 5,

	FORMAT_UNKNOWN	= 6
};


namespace PixelFormat
{
	PIXEL_FORMAT StringToFormat(const std::string& formatName);

	std::string FormatToString(PIXEL_FORMAT format);

}  // namespace PixelFormat


#endif //_PIXELFORMAT_H
