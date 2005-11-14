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
/** The header file for the TestSrcFactory class.
  *
  * @author Denis Kalkofen
  *
  * $Id$
  * @file                                                                   */
 /* ======================================================================= */
#ifndef _TESTSRCFACTORY_H
#define _TESTSRCFACTORY_H

#include "openVideo.h"
#include "TestSrc.h"
#include <core/NodeFactory.h>

/**
*@ingroup nodes
*	A factory to create TestSrc nodes.
*/
class OPENVIDEO_API  TestSrcFactory :
	public NodeFactory
{
public:
	/**
	*	constructor
	*/
	TestSrcFactory(){};

	/**
	*	creates TestSrc nodes
	*/
	virtual const char* getNodeTypeId();
	
	/**
	*	returns TestSrc as the type of known objects
	*/
	virtual TestSrc* createNode();
};


#endif
