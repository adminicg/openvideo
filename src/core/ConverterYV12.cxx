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
 * Graz University of Technology, Institut f�r Maschinelles Sehen und Darstellen,
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

#ifdef OT_USE_AVIFILE64

#include <openvideo/ConverterYV12.h>
#include <avifile-0.7/avifile.h>
#include <avifile-0.7/avm_creators.h>
#include <avifile-0.7/avm_fourcc.h>
#include <avifile-0.7/image.h>
#include <avifile-0.7/videodecoder.h>
#include <memory.h>
#include <iostream>

namespace openvideo {

    ConverterYV12::ConverterYV12()
    {
        using namespace std;
        cerr << "ConverterYV12::ConverterYV12" << endl;

        init();
    }
    
    void
    ConverterYV12::init()
    {
        using namespace std;
        cerr << "ConverterYV12::init()" << endl;

        Converter::init();

        ivdec = NULL;
        bihin = NULL;
        bihout = NULL;
        //outimg = NULL;
    }

    void
    ConverterYV12::deinit()
    {    
        Creators::FreeVideoDecoder(ivdec);

        //if (outimg) delete outimg;
        if (bihin) delete bihin;
        if (bihout) delete bihout;

        Converter::deinit();   
    }

#define RGB888_to_RGB32(r, g, b)		( (unsigned int)( (((r&0xff))<<16) | (((g&0xff))<<8) | (((b&0xff))<<0) ) )

    void
    ConverterYV12::convertToRGB32(const unsigned char* nSrcYUV, int nWidth, int nHeight, unsigned char* nDstRGB32, bool nSwizzle34, int nCropX, int nCropY)
    {
        using namespace std;
        if (!ivdec)
        {
            CodecInfo ci;
            cerr << "CodecInfo generated " << endl;
            ci.fourcc = fccYV12;
            
            if (bihin) delete bihin;
            bihin  = new BitmapInfo(nWidth, nHeight, 16);
            bihin->SetSpace(IMG_FMT_YV12);
            bihin->Print();

            if (bihout) delete bihout;
            bihout = new BitmapInfo(nWidth, nHeight, 32);
            bihout->Print();

            cerr << "BitmapInfo generated " << endl;
            bihout->SetRGB();
            cerr << "before decoder creation " << endl;
            ivdec = Creators::CreateVideoDecoder(*bihin);
            cerr << "decoder created " << endl;
            //outimg = new CImage(bihout,(uint8_t*)nDstRGB32, false);
        }

        CImage outimg(bihout, nDstRGB32, false);

        ivdec->DecodeFrame(&outimg, nSrcYUV, nWidth*nHeight, 1);

    }

    void
    ConverterYV12::convertToRGB24(const unsigned char* nSrcYUV, int nWidth, int nHeight, unsigned char* nDstRGB24, bool nSwizzle34, int nCropX, int nCropY)
    {
        using namespace std;
        if (!ivdec)
        {
            CodecInfo ci;
            cerr << "CodecInfo generated " << endl;
            ci.fourcc = fccYV12;
            
            if (bihin) delete bihin;
            bihin  = new BitmapInfo(nWidth, nHeight, 16);
            bihin->SetSpace(IMG_FMT_YV12);
            //bihin->Print();

            if (bihout) delete bihout;
            bihout = new BitmapInfo(nWidth, nHeight, 24);
            //bihout->Print();

            cerr << "BitmapInfo generated " << endl;
            bihout->SetRGB();
            cerr << "before decoder creation " << endl;
            ivdec = Creators::CreateVideoDecoder(*bihin);
            cerr << "decoder created " << endl;
            //outimg = new CImage(bihout, nDstRGB24, false);
        }

        CImage outimg(bihout, nDstRGB24, false);

        ivdec->DecodeFrame(&outimg, nSrcYUV, nWidth*nHeight, 1);

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

#endif // ifdef OT_USE_AVIFILE64

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
