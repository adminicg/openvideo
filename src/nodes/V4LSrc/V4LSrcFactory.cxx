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
/** The source file for the V4LSrcFactory class.
  *
  * @author Petter Risholm
  * 
  * $Id: 
  * @file                                                                   
 /* ======================================================================= */

#include "V4LSrcFactory.h"

#ifdef ENABLE_V4LSRC

V4LSrcFactory::V4LSrcFactory()
{
}


V4LSrcFactory::~V4LSrcFactory()
{

}

V4LSrc*
V4LSrcFactory::createNode()
{
  return new V4LSrc;

}

const char* 
V4LSrcFactory::getNodeTypeId()
{
  return "V4LSrc";
}

#endif // ENABLE_V4LSRC
