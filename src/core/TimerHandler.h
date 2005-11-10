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
/** The header file for the TimerHandler class.
  *
  * @author Denis Kalkofen
  *
  * $Id$
  * @file                                                                   */
 /* ======================================================================= */


#ifndef _TIMERHANDLER_H
#define _TIMERHANDLER_H

#include "openVideo.h"
#include <ace/Event_Handler.h>

/**
*@ingroup core
*	Class to handle time out events. 
*	The function handle_timout will call the previsouly registered callback function 
*	timerCB.
*/


class OPENVIDEO_API TimerHandler : public ACE_Event_Handler
{
public:
	/**
	*	Constructor
	*/
	TimerHandler();

	/**
	*	Destructor
	*/
	~TimerHandler();
	
	/**
	*	The callback function. This functions is called by 'handle_timeout'
	*/  
	void (*timerCB)(void*);

	/**
	*	The callback data.
	*/
	void* data;

	/**
	*	The function which is called by the Timer.
	*/
	int handle_timeout (const ACE_Time_Value &current_time,
                        const void *);


};

#endif
