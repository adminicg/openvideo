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
/** The source file for the TimerACE class.
  *
  * @author Denis Kalkofen
  * 
  * $Id$
  * @file                                                                   
  * ======================================================================= */

#include <openvideo/TimerACE.h>
#include <ace/Reactor.h>
#include <openvideo/TimerHandlerACE.h>

namespace openvideo{

// constructor
    TimerACE::TimerACE() 
{
	htimer=NULL;
    reactor = (void *)new ACE_Reactor;
}

// destructor

TimerACE::~TimerACE()
{
   /* if(reactor)
    {   
        ((ACE_Reactor*)reactor)->end_reactor_event_loop();
            
    }*/
    // all other ways to stop the reactor result in errors
    ((ACE_Reactor*)reactor)->cancel_timer(htimer);
    ((ACE_Reactor*)reactor)->close();
    if(htimer)
        delete htimer;
}


void 
TimerACE::schedule(void (*timerCB)(void*),void* data,double interval)
{
	htimer=new openvideo::TimerHandlerACE();
	htimer->timerCB=timerCB;
	htimer->data=data;
	ACE_Time_Value initialDelay (0);
	ACE_Time_Value tickVal;
	tickVal.set(interval);
    ((ACE_Reactor*)reactor)->schedule_timer(htimer,
        0,
        initialDelay,
        tickVal);
	//ACE_Reactor::instance()->schedule_timer (htimer,
	//										0,
	//										initialDelay,
	//										tickVal);

}

void 
TimerACE::runEventLoop()
{
    // calling this method starts an infinite while loop
    ((ACE_Reactor*)reactor)->run_reactor_event_loop();
}
}//namespace openvideo{
