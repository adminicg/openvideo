/* ========================================================================
 * Copyright (C) 2004-2006  Graz University of Technology
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
 * For further information please contact Dieter Schmalstieg under
 * <schmalstieg@icg.tu-graz.ac.at> or write to Dieter Schmalstieg,
 * Graz University of Technology, Institut für Maschinelles Sehen und Darstellen,
 * Inffeldgasse 16a, 8010 Graz, Austria.
 * ========================================================================
 * PROJECT: OpenVideo
 * ======================================================================== */
/** @author   Daniel Wagner, modified by Bernhard Reitinger
 *
 * $Id$
 * @file                                                                   */
/* ======================================================================= */

//
// Optimized Converter from YV12 to RGB565 and Luminance
// Written from scratch by Daniel Wagner
// For questions send a mail to: daniel@icg.tu-graz.ac.at
//
// Modified by Bernhard Reitinger in order to convert YV12 to RGBA
//
// Highly optimized C++ version. Uses look-up
// tables for almost everything; thereby requires
// no multiplication or if operations for doing
// format conversion including saturation checks.
//
// Memory usage: allocates 3249 bytes for lookup tables.
//
// Should run pretty fast on any device. Intel IPP
// probably includes a much faster version using WirelessMXX
// (would only work on Intel XScale processors).
//
//

#ifndef _CONVERTERYV12_H_
#define _CONVERTERYV12_H_

namespace openvideo {
    
    class ConverterYV12
    {
    public:

	enum FORMAT {
	    FORMAT_INVALID = 0,
	    FORMAT_RGB565  = 1,
	    FORMAT_RGB555  = 2,
	    FORMAT_RGB24   = 3,
	    FORMAT_RGB32   = 4,
	    FORMAT_YV12    = 5
	};
	
	enum {
	    LUTCAP_MIN = -153,
	    LUTCAP_MAX = 535,
	    CHANNEL_RANGE = 256
	};

	ConverterYV12()  {  init();  }

	~ConverterYV12()  {  deinit();  }

	void convertToRGB32(const unsigned char* nSrcYUV, int nWidth, int nHeight, unsigned int* nDstRGB32, bool nSwizzle34, int nCropX=0, int nCropY=0);

	void convertToLum(const unsigned char* nSrcYUV, int nWidth, int nHeight, unsigned char* nDstLum, bool nSwizzle34, int nCropX=0, int nCropY=0);

    protected:
	void init();
	void deinit();

	int cap(int nV)					{  /*assert(nV>=LUTCAP_MIN && nV<=LUTCAP_MAX);*/  return lutCap[nV];  }

	int getV_for_Red(int nV)		{  /*assert(nV>=0 && nV<CHANNEL_RANGE);*/  return lutV_for_Red[nV];  }
	int getU_for_Blue(int nU)		{  /*assert(nU>=0 && nU<CHANNEL_RANGE);*/  return lutU_for_Blue[nU];  }
	int getV_for_Green(int nV)		{  /*assert(nV>=0 && nV<CHANNEL_RANGE);*/  return lutV_for_Green[nV];  }
	int getU_for_Green(int nU)		{  /*assert(nU>=0 && nU<CHANNEL_RANGE);*/  return lutU_for_Green[nU];  }
	int getY(int nY)				{  /*assert(nY>=0 && nY<CHANNEL_RANGE);*/  return lutY[nY];  }

	unsigned char	*lutCap0,
	    *lutCap;

	short			*lutV_for_Red,
	    *lutU_for_Blue,
	    *lutV_for_Green,
	    *lutU_for_Green,
	    *lutY;
    };


}  // namespace openvideo

#endif // _CONVERTERYV12_H_

//========================================================================
// End of $FILENAME$
//========================================================================
// Local Variables:
// mode: c++
// c-basic-offset: 4
// eval: (c-set-offset 'substatement-open 0)
// eval: (c-set-offset 'case-label '+)
// eval: (c-set-offset 'statement 'c-lineup-runin-statements)
// eval: (setq indent-tabs-mode nil)
// End:
//========================================================================
