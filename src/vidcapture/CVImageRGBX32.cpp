// CVImageRGBX32 - 24-bit RGB image class
// Written by Michael Ellison
//-------------------------------------------------------------------------
//                      CodeVis's Free License
//                         www.codevis.com
//
// Copyright (c) 2003 by Michael Ellison (mike@codevis.com)
// All rights reserved.
//
// You may use this software in source and/or binary form, with or without
// modification, for commercial or non-commercial purposes, provided that 
// you comply with the following conditions:
//
// * Redistributions of source code must retain the above copyright notice,
//   this list of conditions and the following disclaimer. 
//
// * Redistributions of modified source must be clearly marked as modified,
//   and due notice must be placed in the modified source indicating the
//   type of modification(s) and the name(s) of the person(s) performing
//   said modification(s).
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED 
// TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS 
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
//---------------------------------------------------------------------------
// Modifications:
//
//---------------------------------------------------------------------------
/// \file CVImageRGBX32.cpp
/// \brief CVImageRGBX32.cpp implements a 24-bit color image class.
///
/// CVImageRGBX32 is a 24-bit color image class derived from CVImage
/// Pixels are 8-bit per channel unsigned chars,
/// and are stored as triplets in R,G,B order.
/// 
/// See CVImage.h for general documentation.
///
/// $RCSfile: CVImageRGBX32.cpp,v $
/// $Date: 2004/02/08 23:47:39 $
/// $Revision: 1.1.1.1 $
/// $Author: mikeellison $

#include <memory.h>
#include "CVUtil.h"
#include "CVFile.h"
#include "CVImageRGBX32.h"
#include "CVUtil.h"

//---------------------------------------------------------------------------
CVImageRGBX32::CVImageRGBX32()
:CVImage()
{
}
//---------------------------------------------------------------------------
CVImageRGBX32::~CVImageRGBX32()
{
   // Parent calls destroy
}

//---------------------------------------------------------------------------
// GetNumChannels
//    Retrieves the number channels per pixel
//---------------------------------------------------------------------------
int CVImageRGBX32::GetNumChannels() const
{
   return 4;
}

//---------------------------------------------------------------------------
// GetBytesPerPixel
//    Retrieves the number of bytes per pixel.
//---------------------------------------------------------------------------
int CVImageRGBX32::GetBytesPerPixel() const
{
   return 4;
}

//--------------------------------------------------------------------------
// GetImageType
//    Retrieves the type of image - see CVIMAGE_TYPE enum in header
//---------------------------------------------------------------------------
CVImage::CVIMAGE_TYPE CVImageRGBX32::GetImageType() const
{
   return CVIMAGE_RGBX32;
}

//---------------------------------------------------------------------------
// GetPNMExtension() retrieves the default file extension for PNM
// file saving. (e.g. ".pgm" for greyscale)
//
// \return const char* - ASCIIZ default file extension, 
//                       including preceeding '.'
// \sa Load(), Save()
//---------------------------------------------------------------------------
const char *CVImageRGBX32::GetPNMExtension() const
{
   //return ".ppm";
	return "";
}

//---------------------------------------------------------------------------
// GetPNMMagicVal() retrieves the magic value for a pnm file
// matching the current image format.
//---------------------------------------------------------------------------
char CVImageRGBX32::GetPNMMagicVal() const
{
   return '0';
}

//---------------------------------------------------------------------------
// GetMaxPixelValue() retrieves the maximum value of any pixel in
// the image.  
//
// In multichannel images (e.g. RGB triplets), it will
// return the maximum value on any of the channels.
//
// All child classes should implement this.
// \param maxValue - reference to max pixel value, set on return.
// \return CVRES result code
// \sa GetPixel(), SetPixel(), CVImage::GetMaxPixel()
//---------------------------------------------------------------------------
CVRES CVImageRGBX32::GetMaxPixelValue(float& maxValue) const
{
   unsigned char maxPixel;   
   CVRES result = GetMaxPixel(maxPixel);   
   maxValue = (float)maxPixel;
   return result;
}

//---------------------------------------------------------------------------
// GetPixel() retrieves the red, green, and blue values for a specified
// pixel as floating points.
//
// This is for convenience and prototyping - for high-speed image
// processing you'll need to work more directly with the image
// buffer.
//
// Within CVImageRGBX32, this returns the red, green, and blue values
// all of which will be between 0-255.
//
// \param x - x position within the image of the pixel
// \param y - y position within the image of the pixel
// \param r - receives the red value of the pixel
// \param g - receives the green value of the pixel
// \param b - receives the blue value of the pixel
//
// \return CVRES result code.  CVRES_SUCCESS on success.
// \sa SetPixel()
//---------------------------------------------------------------------------
CVRES CVImageRGBX32::GetPixel( int      x,
                              int      y,
                              float&   r,
                              float&   g,
                              float&   b) const
{
   CVRES result = CVRES_SUCCESS;
   
   CVAssert(fData != 0, "Image must be created first!");
   if (fData == 0)
   {
      return CVRES_IMAGE_EMPTY_ERR;
   }

   // Bounds check coordinates
   CVAssert(( (x >= 0) || (x < fWidth)), "X position is out of bounds!");
   CVAssert(( (y >= 0) || (y < fHeight)), "Y position is out of bounds!");

   if ( (x < 0) || (x >= fWidth))
   {
      return CVRES_IMAGE_OUT_OF_RANGE;
   }

   if ( (y < 0) || (y >= fHeight))
   {
      return CVRES_IMAGE_OUT_OF_RANGE;
   }
      
   
   // Offset of pixel on x axis in image data
   int lineOffset = (this->XOffsetAbs() + x) * 4;

   // Absolute length of line in fData in bytes ( >= fWidth * GetBytesPerPixel())
   int lineLength = this->AbsWidth() * 4;
   
   // current position of start of buffer in source
   unsigned char* curPtr = fData + lineOffset + 
                           ((YOffsetAbs() + y) * lineLength);

   // pull out pixels
   r = (float)curPtr[0];
   g = (float)curPtr[1];
   b = (float)curPtr[2];
         
   return result;
}
//---------------------------------------------------------------------------
// SetPixel() sets the red, green, and blue pixel values
// for a pixel
//
// This is for convenience and prototyping - for high-speed image
// processing you'll need to work more directly with the image
// buffer.
//
// Within CVImageRGBX32, the values are truncated to be between
// 0 (min) and 255 (max), then set.      
//
// Intensity values above 255 will be truncated to 255. Values
// below 0 will be set to 0.
//
// \param x - x position within the image of the pixel
// \param y - y position within the image of the pixel
// \param r - receives the red value of the pixel
// \param g - receives the green value of the pixel
// \param b - receives the blue value of the pixel
//
// \return CVRES result code.  CVRES_SUCCESS on success.
// \sa GetPixel()
//---------------------------------------------------------------------------
CVRES CVImageRGBX32::SetPixel (   int      x,
                                 int      y,
                                 float    r,
                                 float    g,
                                 float    b)
{
   CVRES result = CVRES_SUCCESS;
   
   CVAssert(fData != 0, "Image must be created first!");
   if (fData == 0)
   {
      return CVRES_IMAGE_EMPTY_ERR;
   }

   // Bounds check coordinates
   CVAssert(( (x >= 0) || (x < fWidth)), "X position is out of bounds!");
   CVAssert(( (y >= 0) || (y < fHeight)), "Y position is out of bounds!");

   if ( (x < 0) || (x >= fWidth))
   {
      return CVRES_IMAGE_OUT_OF_RANGE;
   }

   if ( (y < 0) || (y >= fHeight))
   {
      return CVRES_IMAGE_OUT_OF_RANGE;
   }
   
   // Offset of pixel on x axis in image data
   int lineOffset = (this->XOffsetAbs() + x) * 4;

   // Absolute length of line in fData in bytes ( >= fWidth * GetBytesPerPixel())
   int lineLength = this->AbsWidth() * 4;
   
   // current position of start of buffer in source
   unsigned char* curPtr = fData + lineOffset + 
                           ((YOffsetAbs() + y) * lineLength);
   
   // Bounds check the pixel values
   r = CVMax( 0.0f,   r );
   r = CVMin( 255.0f, r );
   g = CVMax( 0.0f,   g );
   g = CVMin( 255.0f, g );
   b = CVMax( 0.0f,   b );
   b = CVMin( 255.0f, b );

   // Store
   curPtr[0] = (unsigned char)r;
   curPtr[1] = (unsigned char)g;
   curPtr[2] = (unsigned char)b;
         
   return result;
}

#ifdef WIN32
//---------------------------------------------------------------------------
// SetFromWin32Bmp()
//    Sets the image from a bitmap buffer
//    We do a full copy of the data for this, since we may flip it and
//    swap red with blue to get it into RGB order rather than Window's BGR.
//    Padding will be removed as well.
//
//    Only supports 24-bit RGB bitmaps.
//
//---------------------------------------------------------------------------
CVRES CVImageRGBX32::SetFromWin32Bmp(   const BITMAPINFOHEADER* bmih, 
                                       const unsigned char*    data)
{  
   CVAssert(fData == 0, 
      "SetFromWin32Bmp requires a clean, uninitialized,"\
      " but instantiated image");

   // Parent does sanity checks (only called from CreateFromWin32bmp)  
   CVRES result = CVRES_SUCCESS;

   // Negative height means top down. 
   // Flipped is set if bottom-up image (default for win32)
   bool flipped = bmih->biHeight >= 0;

   // Create an image of the same size 
   // Make sure to use a positive height based on flipped var
   // Note: this adds our reference
   Create(  bmih->biWidth, 
                  flipped?(bmih->biHeight):-(bmih->biHeight), 
                  false);
      
   // Set our starting position in the source image and step pos
   //unsigned char* dstPos   = fData;
   unsigned int* dstPosI   = reinterpret_cast<unsigned int*>(fData);
   unsigned char* srcPos   = 0;

   int srcStep    = -((int)bmih->biSizeImage / fHeight);    

   if (flipped)
   {
      // Set to point to bottom row in image so we can flip it
      srcPos  = (unsigned char*)data + (int)bmih->biSizeImage + srcStep;
   }
   else
   {
      // Start at top if not flipped
      srcPos  = (unsigned char*)data;     
   }

   // Copy row by row from source image to destination image
         
   int y;
   for (y = 0; y < fHeight; y++)
   {     
      unsigned char* srcLine = srcPos;
      {              
         int x;
         for (x = 0; x < fWidth; x++)
         {
			//
			// TODO: this can still be improved by reading 3 DWORDS (four source pixels) at once
			//
			*dstPosI++ = srcLine[2] | (srcLine[1]<<8) | (srcLine[0]<<16) | (255<<24);
            srcLine +=3;
         }
      }        
      srcPos += srcStep;
   }        
   
   // end profiling - print time
   //CVEndTime();

   return result;
}
#endif // WIN32

