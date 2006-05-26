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
/** The source file for the OpenCVSrc class.
 *
 * @author Jorn Skaarud Karlsen
 * 
 * $Id$
 * @file                                                                   
 /* ======================================================================= */

// OpenVideo includes for this file
#include <openvideo/nodes/OpenCVSrc.h>
#include <openvideo/openVideo.h>

// If OpenCV is available
#ifdef ENABLE_OPENCV

// Standard includes
#include <string>
#include <iostream>
#include <sstream>
#include <vector>

// OpenCV includes
#include <cv.h>
#include <cvaux.h>
#include <highgui.h>

// OpenVideo includes
#include <openvideo/nodes/OpenCVSrc.h>
#include <openvideo/Manager.h>
#include <openvideo/State.h>

// We are using namespaces
using namespace std;
using namespace openvideo;

// ********************************************************
// PRIVATE CLASS DEFINITION
// ********************************************************

class OpenCVSrcP
{
public:
  // Constructor
  OpenCVSrcP(OpenCVSrc * owner) {
    this->owner = owner;

    this->captureFrom = CAPTURE_FROM_CAMERA;
    this->capture = NULL;
    
    this->filename = "capture.avi";
    this->cameraId = 0;
    this->width    = 640;
    this->height   = 480;
    this->fps      = 15;
  }
  
  /// Destructor
  ~OpenCVSrcP(void) {
    if (capture != NULL) {
      cvReleaseCapture(&capture);
    }
  }

  /// Type of capture (either camera or file)
  int captureFrom;
  /// Filename of video source 
  string filename;
  /// Camera id
  int cameraId;
  /// Width of video stream
  int width;
  /// Height of video stream
  int height;
  /// Framerate of video stream
  int fps;

  /// CvCapture
  CvCapture * capture;

  // Capture type variables
  const static int CAPTURE_FROM_CAMERA = 1;
  const static int CAPTURE_FROM_FILE   = 2;
  
private:
  /// Parent class
  OpenCVSrc * owner;
};

#undef PUBLIC
#undef PRIVATE

#define PUBLIC(obj) (obj->owner)
#define PRIVATE(obj) (obj->pimpl)

// ********************************************************


// constructor
OpenCVSrc::OpenCVSrc()
{
#if OPENVIDEO_DEBUG && 0
  cout << "OpenCVSrc::OpenCVSrc()" << endl;
#endif

  // Initialize private class
  PRIVATE(this) = new OpenCVSrcP(this);
}

// destructor
OpenCVSrc::~OpenCVSrc()
{
#if OPENVIDEO_DEBUG && 0
  cout << "OpenCVSrc::~OpenCVSrc()" << endl;
#endif
}

/**
 * Initializes the pixel formats
 */
void 
OpenCVSrc::initPixelFormats()
{
#if OPENVIDEO_DEBUG && 0
  cout << "OpenCVSrc::initPixelFormats()" << endl;
#endif

  inherited::pixelFormats.push_back(PIXEL_FORMAT(FORMAT_B8G8R8));
}

/** 
 * Sets the parameters from the XML-file
 */
bool 
OpenCVSrc::setParameter(string key, string value)
{
#if OPENVIDEO_DEBUG && 0
  cout << "OpenCVSrc::setParameter(): " << key << " " << value << endl;
#endif

  // Setup a string stream to read variables from the value string
  istringstream iss(value);

  // Is the key handled by the parent?
  if (inherited::setParameter(key, value)) {
    return true;
  }
  
  // Need to handle the key for our selves.
  if (key == "capturefrom") {
    if (value == "camera") {
      PRIVATE(this)->captureFrom = OpenCVSrcP::CAPTURE_FROM_CAMERA;
    }
    else if (value == "file") {
      PRIVATE(this)->captureFrom = OpenCVSrcP::CAPTURE_FROM_FILE;
    }
    else {
      cerr << "Unknown capture from value: " << value << endl;
      assert(0);
    }
    return true;
  }
  else if (key == "width") {
    iss >> PRIVATE(this)->width;
    return true;
  }
  else if (key == "height") {
    iss >> PRIVATE(this)->height;
    return true;
  }
  else if (key == "fps") {
    iss >> PRIVATE(this)->fps;
    return true;
  }
  else if (key == "cameraId") {
    iss >> PRIVATE(this)->cameraId;
    return true;
  }
  else if (key == "filename") {
    iss >> PRIVATE(this)->filename;
    return true;
  }
  else { // Key not recognized by any system
    cout << "OpenCVSrc::setParameter(): Unable to parse key: '" << key << "' with value: '" << value << "'" << endl;
  }

  // Return false if no system was able to recognize the key
  return false;
}

/**
 * Initializes the video stream
 */
void 
OpenCVSrc::init()
{
#if OPENVIDEO_DEBUG && 0
  cout << "OpenCVSrc::init()" << endl;
#endif

  // Make sure the stream-tree is correctly defined
  assert(inherited::getInDegree() == 0);

  // Initialize state variable
  inherited::state = new State();
  inherited::state->clear();
  inherited::state->width = PRIVATE(this)->width;
  inherited::state->height = PRIVATE(this)->height;
}

/**
 * Starts the capturing thread
 */
void 
OpenCVSrc::start() 
{
#if OPENVIDEO_DEBUG && 0
  cout << "OpenCVSrc::start()" << endl;
#endif 

  // Set up capture process
  if (PRIVATE(this)->captureFrom == OpenCVSrcP::CAPTURE_FROM_FILE) {
    // FIXME: Use cvCaptureFromAVI instead?
    PRIVATE(this)->capture = cvCaptureFromFile(PRIVATE(this)->filename.c_str());
  }
  else if (PRIVATE(this)->captureFrom == OpenCVSrcP::CAPTURE_FROM_CAMERA) { // Capture from video camera(s)
    PRIVATE(this)->capture = cvCaptureFromCAM(PRIVATE(this)->cameraId);
  }

  // Check if the capture object is invalid...
  if (PRIVATE(this)->capture == NULL) {
    cerr << "Unable to open input video stream" << endl;
  }
  else {
    // Set user-defined height and width
    cvSetCaptureProperty(PRIVATE(this)->capture, CV_CAP_PROP_FRAME_WIDTH, PRIVATE(this)->width);
    cvSetCaptureProperty(PRIVATE(this)->capture, CV_CAP_PROP_FRAME_HEIGHT, PRIVATE(this)->height);
    cvSetCaptureProperty(PRIVATE(this)->capture, CV_CAP_PROP_FPS, PRIVATE(this)->fps);

    // Retrieve actual values used
    PRIVATE(this)->width  = (int) cvGetCaptureProperty(PRIVATE(this)->capture, CV_CAP_PROP_FRAME_WIDTH);
    PRIVATE(this)->height = (int) cvGetCaptureProperty(PRIVATE(this)->capture, CV_CAP_PROP_FRAME_HEIGHT);
    PRIVATE(this)->fps    = (int) cvGetCaptureProperty(PRIVATE(this)->capture, CV_CAP_PROP_FPS);

    // FIXME: Have to fake width here because wrong capture property is
    // sometimes returned.
    if (PRIVATE(this)->width == 0) {
      PRIVATE(this)->width = 640;
    }
    if (PRIVATE(this)->height == 0) {
      PRIVATE(this)->height = 480;
    }
  }
}

/**
 * Stops the capturing thread
 */
void
OpenCVSrc::stop() 
{
#if OPENVIDEO_DEBUG && 0
  cout << "OpenCVSrc::stop()" << endl;
#endif 

  // Close down capture process
  cvReleaseCapture(&PRIVATE(this)->capture);
  PRIVATE(this)->capture = NULL;
}

/**
 * Processes the video stream
 */
void 
OpenCVSrc::process()
{
#if OPENVIDEO_DEBUG && 0
  cout << "OpenCVSrc::process()" << endl;
#endif

  IplImage * image = cvQueryFrame(PRIVATE(this)->capture);
  if (image) {
    inherited::state->width  = image->width;
    inherited::state->height = image->height;
    inherited::state->format = FORMAT_R8G8B8;
    inherited::state->frame  = (unsigned char *) image->imageData;
  }
  else {
    cerr << "OpenCVSrc::process(): ERROR: Unable to capture image..." << endl;
  }
}

/**
 * Postprocesses after processing
 */
void
OpenCVSrc::postProcess()
{
#if OPENVIDEO_DEBUG && 0
  cout << "OpenCVSrc::postProcess()" << endl;
#endif 

  this->state->frame = NULL;
}

#endif // ENABLE_OPENCV
