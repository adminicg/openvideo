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
/** The header file for the VideoSink class.
  *
  * @author Denis Kalkofen
  *
  * $Id: VideoSink.h 30 2005-12-10 12:10:50Z denis $
  * @file                                                                   */
 /* ======================================================================= */
#ifndef _VIDEO_SINK_H
#define _VIDEO_SINK_H
#include "openVideo.h"
#ifdef  ENABLE_VIDEOSINK



#include "core/Node.h"

/**
*@ingroup nodes
*	This node implements an publish-subsribers pattern to pass the video to any number of registered consumers.
*	The node can be used to integrate an OpenVideo sink into another programm. 
*
*   Supported Pixel Formats:
*	@li: R8G8B8
*	@li: B8G8R8
*	@li: R8G8B8X8
*	@li: B8G8R8X8
*	@li: L8
*/

namespace openvideo {
class VideoSinkSubscriber;

class OPENVIDEO_API VideoSink : 
	public openvideo::Node
{
public:
	/**
	*	constructor
	*/
	VideoSink();

	/**
	*	destructor
	*/
	~VideoSink();

	/**
	*	
	*/
	virtual void init();

	/**
	*	
	*/   
	virtual void process();

	/**
	*	indicated whether the sink is started or not.
	*/
	bool isStarted;

	virtual void initPixelFormats();
	
	void subscribe(VideoSinkSubscriber* aSubscriber);
protected:
	/**
	*	Internal texture format. defines the number of color components in the texture. valid values = 1,2,3, or 4.
	*/
	int internalFormat;

	vector<VideoSinkSubscriber*> subsrcibers;
	int size_subscribers;
};

} //namespace openvideo {

#endif // 

#endif
