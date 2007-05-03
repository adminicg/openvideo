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
// Optimized Converter from YV12 to RGB565 and Luninance
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

#include <openvideo/ConverterYV12.h>
#include <memory.h>


namespace openvideo {


#define RGB888_to_RGB32(r, g, b)		( (unsigned int)( (((r&0xff))<<16) | (((g&0xff))<<8) | (((b&0xff))<<0) ) )

void
ConverterYV12::convertToRGB32(const unsigned char* nSrcYUV, int nWidth, int nHeight, unsigned int* nDstRGB32, bool nSwizzle34, int nCropX, int nCropY)
{
	//
	// Each U and V value can be used for 4 pixels: (x,y) (x+1,y) (x,y+1) (x+1,y+1)
	// To make best use of that, we therefore do two lines at once.
	// YV12 has one strange specialty: For each 4 pixels in a row, the pixels 3 and 4 are
	// flipped. We therefore hard-code this special behavior by doing 4 pixels per line
	// before looping. This sums up to 8 pixels per inner loop and a quite long inner loop!
	//
	// YV12 stores YUV pixel data in the following format:
	// 1st block: Y at full res (8-bit)
	// 2nd block: V at half res (8-bit)
	// 3rd block: U at half res (8-bit)
	//
	// Basic formula to convert YUV to RGB:
	//   R = 1.164*(i-16) + 1.596*(V - 128);
	//   B = 1.164*(i-16) +                   2.018*(U - 128);
	//   G = 1.164*(i-16) - 0.813*(V - 128) - 0.391*(U - 128);
	//
	// Each pixel is first converted to R, G, B (each 8-bits) and then
	// merged into a 16-bits RGB565 pixel. This routine could therefore
	// easily be modified to support any other RGB-based pixel format.
	//
	// The final picture will miss nCropX left *and* right. Width is therefore
	// reduced by 2*nCropX Pixels. Take care that the resulting image must
	// have a width which is a multiple of 4
	//

	const int V_OFFS = nWidth*nHeight;
	const int U_OFFS = V_OFFS*5/4;
	const int croppedWidth = nWidth-2*nCropX;
	const int croppedHeight = nHeight-2*nCropY;
	const int extendedWidth = nWidth+2*nCropX;
	const int cropOffsY = nCropX + (nCropY/2)*nWidth;
	const int cropOffsUV = nCropX/2 + (nCropY/4)*nWidth/2;

	const unsigned char* srcU = nSrcYUV + U_OFFS + cropOffsUV;	// source pointer for U
	const unsigned char* srcV = nSrcYUV + V_OFFS + cropOffsUV;	// source pointer for V
	const unsigned char* srcY0 = nSrcYUV + cropOffsY;			// source pointer for even Y lines
	const unsigned char* srcY1 = nSrcYUV + cropOffsY + nWidth;	// source pointer for odd Y lines

	unsigned int* dst0 = nDstRGB32;							// destination pointer for even lines
	unsigned int* dst1 = nDstRGB32+croppedWidth;				// destination pointer for odd lines

	if(nSwizzle34)
	{
		int yl = croppedHeight/2 + 1;
		while(--yl)							// pre-decrement with test for !=0 is faster than post-increment on ARM processors!
		{
			int xl=croppedWidth/4 + 1;		// pre-decrement with test for !=0 is faster than post-increment on ARM processors!
			while(--xl)
			{
				// each run does 8 pixels: 4 pixels in two lines
				//

				int R,G,B, R0,G0,B0;
				int U,V,Y, Y0;

				// U and V can be used for 4 pixels
				//
				U = *srcU++;
				V = *srcV++;

				R0 = getV_for_Red(V);
				B0 = getU_for_Blue(U);
				G0 = getV_for_Green(V) + getU_for_Green(U);


				Y = srcY0[0];
				Y0 = getY(Y);

				R = cap(Y0 + R0);
				G = cap(Y0 + G0);
				B = cap(Y0 + B0);

				dst0[0] = RGB888_to_RGB32(R,G,B);


				Y = srcY0[1];
				Y0 = getY(Y);

				R = cap(Y0 + R0);
				G = cap(Y0 + G0);
				B = cap(Y0 + B0);

				dst0[1] = RGB888_to_RGB32(R,G,B);


				Y = srcY1[0];
				Y0 = getY(Y);

				R = cap(Y0 + R0);
				G = cap(Y0 + G0);
				B = cap(Y0 + B0);

				dst1[0] = RGB888_to_RGB32(R,G,B);


				Y = srcY1[1];
				Y0 = getY(Y);

				R = cap(Y0 + R0);
				G = cap(Y0 + G0);
				B = cap(Y0 + B0);

				dst1[1] = RGB888_to_RGB32(R,G,B);


				// U and V can be used for 4 pixels
				//
				U = *srcU++;
				V = *srcV++;

				R0 = getV_for_Red(V);
				B0 = getU_for_Blue(U);
				G0 = getV_for_Green(V) + getU_for_Green(U);


				Y = srcY0[3];
				Y0 = getY(Y);

				R = cap(Y0 + R0);
				G = cap(Y0 + G0);
				B = cap(Y0 + B0);

				dst0[2] = RGB888_to_RGB32(R,G,B);


				Y = srcY0[2];
				Y0 = getY(Y);

				R = cap(Y0 + R0);
				G = cap(Y0 + G0);
				B = cap(Y0 + B0);

				dst0[3] = RGB888_to_RGB32(R,G,B);


				Y = srcY1[3];
				Y0 = getY(Y);

				R = cap(Y0 + R0);
				G = cap(Y0 + G0);
				B = cap(Y0 + B0);

				dst1[2] = RGB888_to_RGB32(R,G,B);


				Y = srcY1[2];
				Y0 = getY(Y);

				R = cap(Y0 + R0);
				G = cap(Y0 + G0);
				B = cap(Y0 + B0);

				dst1[3] = RGB888_to_RGB32(R,G,B);


				dst0 += 4;
				dst1 += 4;
				srcY0 += 4;
				srcY1 += 4;
			}

			dst0 += croppedWidth;
			dst1 += croppedWidth;
			srcU += nCropX;
			srcV += nCropX;
			srcY0 += extendedWidth;
			srcY1 += extendedWidth;
		}
	}
	else
	{
		int yl = croppedHeight/2 + 1;
		while(--yl)							// pre-decrement with test for !=0 is faster than post-increment on ARM processors!
		{
			int xl=croppedWidth/4 + 1;		// pre-decrement with test for !=0 is faster than post-increment on ARM processors!
			while(--xl)
			{
				// each run does 8 pixels: 4 pixels in two lines
				//

				int R,G,B, R0,G0,B0;
				int U,V,Y, Y0;

				// U and V can be used for 4 pixels
				//
				U = *srcU++;
				V = *srcV++;

				R0 = getV_for_Red(V);
				B0 = getU_for_Blue(U);
				G0 = getV_for_Green(V) + getU_for_Green(U);


				Y = srcY0[0];
				Y0 = getY(Y);

				R = cap(Y0 + R0);
				G = cap(Y0 + G0);
				B = cap(Y0 + B0);

				dst0[0] = RGB888_to_RGB32(R,G,B);


				Y = srcY0[1];
				Y0 = getY(Y);

				R = cap(Y0 + R0);
				G = cap(Y0 + G0);
				B = cap(Y0 + B0);

				dst0[1] = RGB888_to_RGB32(R,G,B);


				Y = srcY1[0];
				Y0 = getY(Y);

				R = cap(Y0 + R0);
				G = cap(Y0 + G0);
				B = cap(Y0 + B0);

				dst1[0] = RGB888_to_RGB32(R,G,B);


				Y = srcY1[1];
				Y0 = getY(Y);

				R = cap(Y0 + R0);
				G = cap(Y0 + G0);
				B = cap(Y0 + B0);

				dst1[1] = RGB888_to_RGB32(R,G,B);


				// U and V can be used for 4 pixels
				//
				U = *srcU++;
				V = *srcV++;

				R0 = getV_for_Red(V);
				B0 = getU_for_Blue(U);
				G0 = getV_for_Green(V) + getU_for_Green(U);


				Y = srcY0[2];
				Y0 = getY(Y);

				R = cap(Y0 + R0);
				G = cap(Y0 + G0);
				B = cap(Y0 + B0);

				dst0[2] = RGB888_to_RGB32(R,G,B);


				Y = srcY0[3];
				Y0 = getY(Y);

				R = cap(Y0 + R0);
				G = cap(Y0 + G0);
				B = cap(Y0 + B0);

				dst0[3] = RGB888_to_RGB32(R,G,B);


				Y = srcY1[2];
				Y0 = getY(Y);

				R = cap(Y0 + R0);
				G = cap(Y0 + G0);
				B = cap(Y0 + B0);

				dst1[2] = RGB888_to_RGB32(R,G,B);


				Y = srcY1[3];
				Y0 = getY(Y);

				R = cap(Y0 + R0);
				G = cap(Y0 + G0);
				B = cap(Y0 + B0);

				dst1[3] = RGB888_to_RGB32(R,G,B);


				dst0 += 4;
				dst1 += 4;
				srcY0 += 4;
				srcY1 += 4;
			}

			dst0 += croppedWidth;
			dst1 += croppedWidth;
			srcU += nCropX;
			srcV += nCropX;
			srcY0 += extendedWidth;
			srcY1 += extendedWidth;
		}
	}
}


void
ConverterYV12::convertToLum(const unsigned char* nSrcYUV, int nWidth, int nHeight, unsigned char* nDstLum, bool nSwizzle34, int nCropX, int nCropY)
{
	// Converts from YV12 to Luminance (8-bit gray). Luminance is stored in full
	// resolution as the first block in the YV12 image. Unfortunately, for every 
	// four pixels we have to switch pixels 3 and 4.
	//
	// The final picture will miss nCropX left *and* right. Width is therefore
	// reduced by 2*nCropX Pixels. Take care that the resulting image must
	// have a width which is a multiple of 16. nCropX must be an even number.
	//

	const unsigned int* src = reinterpret_cast<const unsigned int*>(nSrcYUV+nCropX+(nCropY/2)*nWidth);		// source pointer
	unsigned int* dst = reinterpret_cast<unsigned int*>(nDstLum);											// destination pointer
	const int numRuns = (nWidth-2*nCropX)/16 + 1;

	int y = nHeight-2*nCropY+1;
	while(--y)
	{
		// the algorithm's inner loop uses 7 variables. so everything
		// should fit into registers on ARM processors...
		//
		int x = numRuns;												// +1 for pre-decriment (faster on ARM processors)
		int v0,v1,v2,v3;

		if(nSwizzle34)
		{
			while(--x)	
			{
				// read 16 pixels
				//
				v0 = *src++;
				v1 = *src++;
				v2 = *src++;
				v3 = *src++;

				// write 16 pixels. do byte swizzling to exchange pixels 3 and 4 of each quadruple
				//
				*dst++ = (v0&0x0000ffff) | ((v0>>8)&0x00ff0000) | ((v0<<8)&0xff000000);
				*dst++ = (v1&0x0000ffff) | ((v1>>8)&0x00ff0000) | ((v1<<8)&0xff000000);
				*dst++ = (v2&0x0000ffff) | ((v2>>8)&0x00ff0000) | ((v2<<8)&0xff000000);
				*dst++ = (v3&0x0000ffff) | ((v3>>8)&0x00ff0000) | ((v3<<8)&0xff000000);
			}
		}
		else
		{
			x--;
			memcpy(dst,src, x*16);
			src += x*4;
			dst += x*4;
		}

		src += nCropX/2;
	}
}


}  // namespace openvideo

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
