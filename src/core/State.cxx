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
/** The source file for the State class.
  *
  * @author Denis Kalkofen, Daniel Wagner
  * 
  * $Id$
  * @file                                                                   
 /* ======================================================================= */


#include <openvideo/State.h>
#include <ACE/Thread_Mutex.h>


namespace openvideo {


void
Buffer::unlock()
{
	lockCtr--;
	assert(lockCtr>=0);
}


void
State::unlockAllBuffers()
{
	for(size_t i=0; i<buffers.size(); i++)
		buffers[i]->unlock();
}


Buffer*
State::findFreeBuffer()
{
	for(size_t i=0; i<buffers.size(); i++)
	{
		if(!buffers[i]->isLocked())
			return buffers[i];
	}

	return NULL;
}


int
State::getNumLockedBuffers() const
{
	int c=0;

	for(size_t i=0; i<buffers.size(); i++)
		if(buffers[i]->isLocked())
			c++;

	return c;
}


BufferSynchronizer::BufferSynchronizer()
{
	buffer = NULL;
	mutex = new ACE_Thread_Mutex;
}


BufferSynchronizer::~BufferSynchronizer()
{
	if(buffer)
		buffer->unlock();
	buffer = NULL;

	delete mutex;
	mutex = NULL;
}


void
BufferSynchronizer::assign(Buffer* newBuffer)
{
	if(buffer == newBuffer)
		return;

	mutex->acquire();

		Buffer* oldBuffer = buffer;
		if(newBuffer)
			newBuffer->lock();
		buffer = newBuffer;
		if(oldBuffer)
			oldBuffer->unlock();

	mutex->release();
}


Buffer*
BufferSynchronizer::getLocked()
{
	Buffer* retBuffer;

	if(!buffer)
		return NULL;

	mutex->acquire();

		retBuffer = buffer;
		retBuffer->lock();

	mutex->release();

	return retBuffer;
}


}  // namespace openvideo
