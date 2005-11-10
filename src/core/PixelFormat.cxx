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


#include "PixelFormat.h"
#include <map>


namespace PixelFormat
{

typedef std::map<std::string, PIXEL_FORMAT>	StringFormatMap;
typedef std::pair<std::string, PIXEL_FORMAT> StringFormatPair;


const char* formatNames[] = {
		"R8G8B8",
		"B8G8R8",
		"R8G8B8X8",
		"B8G8R8X8",
		"R5G6B5",
		"L8",
		"UNKNOWN"
	};

static StringFormatMap stringFormatMap;


static void
fillStringFormatMap()
{
	stringFormatMap.clear();

	for(int i=0; i<FORMAT_UNKNOWN; i++)
		stringFormatMap.insert( StringFormatPair(formatNames[i], static_cast<PIXEL_FORMAT>(i)) );
}


PIXEL_FORMAT
StringToFormat(const std::string& formatName)
{
	if(stringFormatMap.empty())
		fillStringFormatMap();

	StringFormatMap::iterator it = stringFormatMap.find(formatName);

	if(it == stringFormatMap.end())
		return FORMAT_UNKNOWN;

	return it->second;
}


std::string
FormatToString(PIXEL_FORMAT format)
{
	if(format>=0 && format<FORMAT_UNKNOWN)
		return formatNames[format];

	return formatNames[FORMAT_UNKNOWN];
}


}  // namespace PixelFormat
