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
/** The header file for the Timer class.
  *
  * @author Denis Kalkofen
  *
  * $Id$
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _TIMER_H
#define _TIMER_H

#include "openVideo.h"

/**
*@ingroup core
*	Implemtents a Timer, based on the timer implementations the ACE_Reactor provides. 
*	It uses an instance of TimerHandler to schedule the timer. 
*/


namespace openvideo {
class TimerHandler;

class OPENVIDEO_API Timer 
{
public:
	/**
	*	Constructor
	*/	
	Timer();

	/**
	*	Destructor
	*/	
	~Timer();
  
	/**
	*	Schedule the timer.\n 
	*	parameter: \n
	*		a) timerCB - the callback function
	*		b) data - callback data
	*		c) interval - the timers interval
	*/	  
	void schedule(void (*timerCB)(void*),void* data,double interval);

	/**
	*	Start event loop. (calls ACE_Reactor::run_event_loop())
	*/	
	static void runEventLoop();

	/**
	* Boolean to prevent multiple calls to ACE_Reactor::run_event_loop();
	*/
	static bool isEventLoopRunning;

private:
	/**
	*	The timer handler .
	*/
	openvideo::TimerHandler *htimer;

};

}//openvideo {
#endif
