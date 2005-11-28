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
/** The header file for the State class.
  *
  * @author Denis Kalkofen
  *
  * $Id: State.h 15 2005-11-10 19:23:15Z daniel $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _STATE_H
#define _STATE_H
#include "openVideo.h"

#include <map>
#include <string>


/**
*@ingroup core
*	A State is used to pass data along OpenVideo's graph.
*	An OpenTracker node passes exactly one state object to all of its children. 
*	Since each node is allowed to create a new State a node can in turn access multiple states.\n
*	The Statet implementation holds variables specific for video data like (image)width, (image)height, image(format) or the current(frame).
*	On top of this, a State implements the ability to share any arbitraty data in its elements map. Therefore It provides functions to set, access and remove those data from the map. \n
*	The following drawing demonstrates the use of multiple State objects in an OpenVideo graph.
*	
*	\image html Stategraph.gif
*	
*/
namespace openvideo {
class OPENVIDEO_API State
{
public:
	/** 
	*	constructor
    */
    State();    
   	
	/** 
	*	destructor
    */
	~State();
	
	/** 
	*	Store a pointer to a video frame
    */
	unsigned char* frame;

	/** 
	*	Store the image width.
    */
	int width;

	/** 
	*	Store the image height.
    */
	int height;

	/** 
	*	Store the image format
    */
	int format;

	/** 
	*	Clear the entire State.
    */
	void clear();

	/** 
	*	Adds an Element to the element map.
    */
	void  addElement(std::string key,void* value);

	/** 
	*	Returns an element from the map.
    */
	void* getElementData(std::string key);

	/** 
	*	Remove an element from the map.
    */
	void  removeElement(std::string key);

protected:

private:
	/** 
	*	A map to store additional data in the State.
    */

	std::map<std::string,void*> elements;
};
}//namespace openvideo {

// ----> inlines <-----
// constructor method.
using namespace openvideo;

inline State::State()
{
	clear();
}
inline State::~State()
{
	clear();
}

inline void 
State::clear()
{
	elements.clear();
	frame=NULL;
	width=0;
	height=0;
	format=0;
}
inline void 
State::addElement(std::string key,void* value)
{
	elements[key]=value;
}

inline void* 
State::getElementData(std::string key)
{
	std::map<std::string,void*>::iterator it = elements.find( key );
    if( it == elements.end())
        return NULL;
 
	return it->second;
}

inline void 
State::removeElement(std::string key)
{
	elements.erase(key);
}

#endif
