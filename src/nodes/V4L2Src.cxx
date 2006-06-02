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
/** The source file for the V4L2Src class.
 *
 * @author Bernhard Reitinger
 *
 * $Id:
 * @file
 *
 /* ======================================================================= */

#ifdef ENABLE_V4L2SRC

#include <openvideo/nodes/V4L2Src.h>
#include <openvideo/Manager.h>
#include <openvideo/State.h>

#include <cstdio>
#include <cstdlib>
#include <cassert>

#include <fstream>
#include <iostream>

#include <fcntl.h>              /* low-level i/o */
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

#include <asm/types.h>          /* for videodev2.h */
#include <linux/videodev2.h>

// only required for setting framerate manually
#include <linux/videodev.h>

#define CLEAR(x) memset (&(x), 0, sizeof (x))

namespace openvideo {

    // V4L2SrcBuffer gives V4L2Src full access to openvideo::Buffer
    class V4L2SrcBuffer : public Buffer
    {
	friend class V4L2Src;
    public:
	V4L2SrcBuffer(State *state, unsigned char* pixbuffer)
	{
            buffer = pixbuffer;
            format = state->format;
            width = state->width;
            height = state->height;
	}
        
	~V4L2SrcBuffer()
	{
            delete buffer;
            buffer = NULL;
	}

	void incUpdateCounter()  {  updateCtr++;  }
        
    };


    //V4L2SrcState gives V4L2Src full access to openvideo::State
    class V4L2SrcState : public State
    {
    public:
	BufferVector& getBuffers()  {  return buffers;  }
        void setCurrentBuffer(Buffer* buf)  {  currentBuffer = buf;  }
    };
    
#define V4L2_State(_STATE)    reinterpret_cast<V4L2SrcState*>(_STATE)
    
    
    // Initialize variable to default values.
    V4L2Src::V4L2Src() : videoWidth(640), videoHeight(480), fps(30),
                         pixelFormat(FORMAT_R8G8B8X8), videoFd(-1),
                         ioMode(IO_METHOD_MMAP), buffers(NULL), nBuffers(0), 
                         initialized(false)
    {
        strcpy(videoDevice, "/dev/video0");

        // allocate new state
        state = new V4L2SrcState;
    }
    
    V4L2Src::~V4L2Src()
    {
        state->unlockAllBuffers();
        delete state;
    }

    void 
    V4L2Src::init() {

        state->clear();

	Manager::getInstance()->getLogger()->logEx("Resolution = %d %d\n", videoWidth, videoHeight);


        state->width=videoWidth;
        state->height=videoHeight;
	state->format = pixelFormat;

	// make a double buffered state
	Manager::getInstance()->getLogger()->logEx("V4L2Src: Using double buffering\n");        
	for(int i=0; i<2; i++)
	{
            unsigned char *pixels = (unsigned char*)malloc(videoWidth*videoHeight*sizeof(unsigned int));
            // clear buffer
            memset(pixels, 0, videoWidth*videoHeight*sizeof(unsigned int));

            reinterpret_cast<V4L2SrcState*>(state)->getBuffers().push_back(new V4L2SrcBuffer(state, pixels));
	}
        
        // initialization (must be done after setting all parameters)
	Manager::getInstance()->getLogger()->log("OpenVideo: starting driver initialization ... ");
        
        if (openDevice())
            initialized = initDevice();
        
        if (!initialized) return;

        // setting the framerate
        setFrameRate(fps);
	Manager::getInstance()->getLogger()->log("done.\n");
    }
    
    void 
    V4L2Src::initPixelFormats() {
        pixelFormats.push_back(PIXEL_FORMAT(FORMAT_R8G8B8X8));
    }

    void
    V4L2Src::start()
    {
        using namespace std;
        
        unsigned int i;
        enum v4l2_buf_type type;

	switch (ioMode) {
            case IO_METHOD_READ:
                Manager::getInstance()->getLogger()->log("INFO: using standard read method.\n");
		/* Nothing to do. */
		break;
                
            case IO_METHOD_MMAP:
                Manager::getInstance()->getLogger()->log("INFO: using memory mapped file method.\n");
		for (i = 0; i < nBuffers; ++i) {
                    struct v4l2_buffer buf;

                    CLEAR (buf);

                    buf.type            = V4L2_BUF_TYPE_VIDEO_CAPTURE;
                    buf.memory          = V4L2_MEMORY_MMAP;
                    buf.index           = i;

                    if (xioctl (videoFd, VIDIOC_QBUF, &buf) == -1)
                        assert(0);
		}
		
		type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
                
		if (xioctl (videoFd, VIDIOC_STREAMON, &type) == -1)
                    assert(0);

		break;

            case IO_METHOD_USERPTR:
                Manager::getInstance()->getLogger()->log("INFO: using user pointer method.\n");
		for (i = 0; i < nBuffers; ++i) {
                    struct v4l2_buffer buf;

                    CLEAR (buf);

                    buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
                    buf.memory      = V4L2_MEMORY_USERPTR;
                    buf.m.userptr	= (unsigned long) buffers[i].start;
                    buf.length      = buffers[i].length;

                    if (xioctl (videoFd, VIDIOC_QBUF, &buf) == -1)
                        assert(0);
		}
                
		type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

		if (xioctl (videoFd, VIDIOC_STREAMON, &type) == -1)
                    assert(0);
                
		break;
	}
    }

    void
    V4L2Src::process()
    {
        if (!initialized) return;

        using namespace std; 

        if (videoFd < 0) {
            Manager::getInstance()->getLogger()->log("ERROR: no video devices available.\n");
            return;
        }

        struct v4l2_buffer buf;
	unsigned int i;

	switch (ioMode) 
        {
            case IO_METHOD_READ:
    		if (read (videoFd, buffers[0].start, buffers[0].length) == -1)
                {
                    switch (errno) 
                    {
            		case EAGAIN:
                            return;
                            
			case EIO:
                            /* Could ignore EIO, see spec. */
                            
                            /* fall through */
                            
			default:
                            assert(0);
                    }
		}
                
    		processImage ((char*)buffers[0].start);
                
		break;
                
            case IO_METHOD_MMAP:
		CLEAR (buf);
                
            	buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
            	buf.memory = V4L2_MEMORY_MMAP;
                
    		if (xioctl (videoFd, VIDIOC_DQBUF, &buf) == -1) {
                    switch (errno) 
                    {
            		case EAGAIN:
                            return;
                            
			case EIO:
                            /* Could ignore EIO, see spec. */
                            
                            /* fall through */
                            
			default:
                            assert(0);
                    }
		}
                
                assert (buf.index < nBuffers);
                
	        processImage ((char*)buffers[buf.index].start);

		if (xioctl (videoFd, VIDIOC_QBUF, &buf) == -1)
                    assert(0);

		break;
                
            case IO_METHOD_USERPTR:
		CLEAR (buf);

    		buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    		buf.memory = V4L2_MEMORY_USERPTR;
                
		if (xioctl (videoFd, VIDIOC_DQBUF, &buf) == -1) 
                {
                    switch (errno) 
                    {
			case EAGAIN:
                            return;
                            
			case EIO:
                            /* Could ignore EIO, see spec. */
                            
                            /* fall through */
                            
			default:
                            assert(0);
                    }
		}
                
		for (i = 0; i < nBuffers; ++i)
                    if (buf.m.userptr == (unsigned long) buffers[i].start
                        && buf.length == buffers[i].length)
                        break;
                
		assert (i < nBuffers);

    		processImage ((void *) buf.m.userptr);
                
		if (xioctl (videoFd, VIDIOC_QBUF, &buf) == -1)
                    assert(0);
                
		break;
	}

    }

    void
    V4L2Src::processImage(const void* p) 
    {
	V4L2SrcBuffer* buffer = reinterpret_cast<V4L2SrcBuffer*>(state->findFreeBuffer());

	if(!buffer)
	{
            Manager::getInstance()->getLogger()->log("OpenVideo::V4L2Src all buffers locked, can not read a new camera image!\n");
            return;
	}
	unsigned char* img = const_cast<unsigned char*>(buffer->getPixels());

        // convert from YUV420 to RGBA32
        converter.convertToRGB32((unsigned char*)p, videoWidth, videoHeight, reinterpret_cast<unsigned int*>(img), false);


//         for (int xxx=0; xxx<videoWidth*videoHeight*sizeof(unsigned int); xxx++)
//         {
//             img[xxx] = 128+buffer->getUpdateCounter();
//         }
            
        V4L2_State(state)->setCurrentBuffer(buffer);
//         printf("swapping buffers ...\n");
        
   	buffer->incUpdateCounter();
    }

    void
    V4L2Src::postProcess()
    {
    }

    bool
    V4L2Src::setParameter(std::string key, std::string value)
    {
        using namespace std;
	Manager::getInstance()->getLogger()->logEx("V4L2Src::setParameter: %s %s\n", key.c_str(), value.c_str());
        if(Node::setParameter(key,value))
            return true;


        if(key=="width")
        {
            this->videoWidth=atoi(value.c_str());
            return true;
        }
        else if(key=="height")
        {
            this->videoHeight=atoi(value.c_str());
            return true;
        }
        else if (key=="device")
        {
            strcpy(this->videoDevice, value.c_str());
            return true;
        }
        else if (key=="fps")
        {
            this->fps=atoi(value.c_str());
            return true;
        }
        else if (key=="format")
        {
            PIXEL_FORMAT pf = PixelFormat::StringToFormat(value);

            switch(pf) {
                case (FORMAT_R8G8B8X8):
                    pixelFormat = pf;
                    break;
                default:
                    cout << "PixelFormat " << value << " is not supported. "
                         << "Using default value " << PixelFormat::FormatToString(pixelFormat) << endl;
            }
      
            return true;
        }
        else
        {
            cout << "No key: " << key << endl;
        }
        return false;
    }


    bool
    V4L2Src::openDevice() 
    {
        using namespace std;
        
        struct stat st; 
        
        if (videoFd != -1) 
        {
            closeDevice();
        }
        
        if (stat(videoDevice, &st) == -1) 
        {
            cerr << "ERROR: cannot identify given device " << videoDevice << endl;
            return false;
        }

        if (!S_ISCHR (st.st_mode)) {
            cerr << "ERROR: given device is not a real device" << endl;
            return false;
        }
        
        videoFd = open(videoDevice, O_RDWR /* required */ | O_NONBLOCK, 0);
        
        if (videoFd == -1) {
            cerr << "ERROR: cannot open device" << endl;
            return false;
        }
        
    }

    bool
    V4L2Src::initDevice() 
    {
        using namespace std; 

        struct v4l2_capability cap;
        struct v4l2_cropcap cropcap;
        struct v4l2_crop crop;
        struct v4l2_format fmt;
	unsigned int min;
        
        if (xioctl (videoFd, VIDIOC_QUERYCAP, &cap) == -1) {
            if (errno == EINVAL) {
                cerr << "ERROR: " << videoDevice << " is no V4L2 device" << endl;
                return false;
            }
            return false;
        }
        
        if (!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE)) {
            cerr << "ERROR: " << videoDevice << " is no video capture device" << endl;
            return false;
        }
        
	switch (ioMode) {
            case IO_METHOD_READ:
		if (!(cap.capabilities & V4L2_CAP_READWRITE)) {
                    cerr << "ERROR: " << videoDevice << " does not support read i/o" << endl;
                    return false;
		}
                break;
                
            case IO_METHOD_MMAP:
            case IO_METHOD_USERPTR:
		if (!(cap.capabilities & V4L2_CAP_STREAMING)) {
                    cerr << "ERROR: " << videoDevice << " does not support streaming i/o" << endl;
                    return false;
		}
		break;
	};


        /* Select video input, video standard and tune here. */


	CLEAR (cropcap);

        cropcap.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        
        if (xioctl (videoFd, VIDIOC_CROPCAP, &cropcap) == 0) {
            crop.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
            crop.c = cropcap.defrect; /* reset to default */
            
            if (xioctl (videoFd, VIDIOC_S_CROP, &crop) == -1) {
                switch (errno) {
                    case EINVAL:
                        /* Cropping not supported. */
                        break;
                    default:
                        /* Errors ignored. */
                        break;
                }
            }
        } else {	
            /* Errors ignored. */
        }

        
        CLEAR (fmt);

        fmt.type                = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        fmt.fmt.pix.width       = videoWidth; 
        fmt.fmt.pix.height      = videoHeight;
        fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
        fmt.fmt.pix.field       = V4L2_FIELD_INTERLACED;

        if (xioctl (videoFd, VIDIOC_S_FMT, &fmt) == -1)
            return false;
        
        /* Note VIDIOC_S_FMT may change width and height. */

	/* Buggy driver paranoia. */
	min = fmt.fmt.pix.width * 2;
	if (fmt.fmt.pix.bytesperline < min)
            fmt.fmt.pix.bytesperline = min;
	min = fmt.fmt.pix.bytesperline * fmt.fmt.pix.height;
	if (fmt.fmt.pix.sizeimage < min)
            fmt.fmt.pix.sizeimage = min;
        
	switch (ioMode) {
            case IO_METHOD_READ:
		return initIORead (fmt.fmt.pix.sizeimage);
		break;
                
            case IO_METHOD_MMAP:
		return initIOMMap ();
		break;
                
            case IO_METHOD_USERPTR:
		return initIOUserp (fmt.fmt.pix.sizeimage);
		break;
	};
    }

    bool 
    V4L2Src::initIORead(unsigned int bufferSize)
    {
        using namespace std; 
        buffers = (buffer*)calloc (1, sizeof (*buffers));
        
        if (!buffers) {
            cerr << "ERROR: Out of memory" << endl;
            return false;
        }

	buffers[0].length = bufferSize;
	buffers[0].start = malloc(bufferSize);
        
	if (!buffers[0].start) {
            cerr << "ERROR: Out of memory" << endl;
            return false;
	}
    }

    bool V4L2Src::initIOMMap()
    {
        using namespace std; 
	struct v4l2_requestbuffers req;

        CLEAR (req);
        
        req.count               = 4;
        req.type                = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        req.memory              = V4L2_MEMORY_MMAP;

	if (xioctl (videoFd, VIDIOC_REQBUFS, &req) == -1) 
        {
            if (EINVAL == errno) 
                cerr << "ERROR: " << videoDevice << " does not support memory mapping" << endl;
            return false;
        }

        if (req.count < 2) 
        {
            cerr << "ERROR: Insufficient buffer memory" << endl;
            return false;
        }

        buffers = (buffer*)calloc (req.count, sizeof (*buffers));

        if (!buffers) {
            cerr << "ERROR: Out of memory" << endl;
            return false;
        }
        
        for (nBuffers = 0; nBuffers < req.count; ++nBuffers) {
            struct v4l2_buffer buf;
            
            CLEAR (buf);
            
            buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
            buf.memory      = V4L2_MEMORY_MMAP;
            buf.index       = nBuffers;
            
            if (xioctl (videoFd, VIDIOC_QUERYBUF, &buf) == -1)
                assert(0);

            buffers[nBuffers].length = buf.length;
            buffers[nBuffers].start =
                mmap (NULL /* start anywhere */,
                      buf.length,
                      PROT_READ | PROT_WRITE /* required */,
                      MAP_SHARED /* recommended */,
                      videoFd, buf.m.offset);
            
            if (MAP_FAILED == buffers[nBuffers].start)
                assert(0);
        }
    }
    
    bool 
    V4L2Src::initIOUserp(unsigned int bufferSize)
    {
        using namespace std; 
	struct v4l2_requestbuffers req;
        
        CLEAR (req);
        
        req.count               = 4;
        req.type                = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        req.memory              = V4L2_MEMORY_USERPTR;

        if (xioctl (videoFd, VIDIOC_REQBUFS, &req) == -1) {
            if (EINVAL == errno) 
                cerr << "ERROR: " << videoDevice << " does not support user pointer i/o" << endl;
            return false;
        }

        buffers = (buffer*)calloc (4, sizeof (*buffers));

        if (!buffers) {
            cerr << "ERROR: Out of memory" << endl;
            return false;
        }
        
        for (nBuffers = 0; nBuffers < 4; ++nBuffers) {
            buffers[nBuffers].length = bufferSize;
            buffers[nBuffers].start = malloc (bufferSize);

            if (!buffers[nBuffers].start) {
                cerr << "ERROR: Out of memory" << endl;
                return false;
            }
        }
    }
    
    void
    V4L2Src::closeDevice() 
    {
        using namespace std; 
        unsigned int i;
        
	switch (ioMode) {
            case IO_METHOD_READ:
		free(buffers[0].start);
		break;
                
            case IO_METHOD_MMAP:
		for (i = 0; i < nBuffers; ++i)
                    if (-1 == munmap (buffers[i].start, buffers[i].length))
                        assert(0);
		break;
                
            case IO_METHOD_USERPTR:
		for (i = 0; i < nBuffers; ++i)
                    free (buffers[i].start);
		break;
	}
        
	free(buffers);
        
        if (close (videoFd) == -1)
            cerr << "ERROR: Error while closing the device" << endl;
        
        videoFd = -1;

//         cerr << "Device closed successfully." << endl;
    }

    void
    V4L2Src::stop() 
    {
        initialized = false;

        enum v4l2_buf_type type;

	switch (ioMode) {
            case IO_METHOD_READ:
		/* Nothing to do. */
		break;
                
            case IO_METHOD_MMAP:
            case IO_METHOD_USERPTR:
		type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

		if (xioctl (videoFd, VIDIOC_STREAMOFF, &type) == -1)
                    assert (0);
                break;
	}

        closeDevice();
    }

    int
    V4L2Src::xioctl(int fd, int request, void *arg)
    {
        int r;
        
        do r = ioctl (fd, request, arg);
        while (r == -1 && errno == EINTR);
        
        return r;
    }

    void
    V4L2Src::setFrameRate(int fps) 
    {
	struct video_window vwin;
        
	/* get resolution/framerate */
	if (ioctl(videoFd, VIDIOCGWIN, &vwin) == -1)
            return;
        
	if (vwin.flags & V4L2_FPS_FRMASK)
	{
            /* set new framerate */
            vwin.flags &= ~V4L2_FPS_FRMASK;
            vwin.flags |= (fps << V4L2_FPS_SHIFT);
            
            if (ioctl(videoFd, VIDIOCSWIN, &vwin) == -1)
                return;
	}
	else
	{
            fprintf(stderr, "This device doesn't support setting the framerate.\n");
            exit(1);
	}
    }
    

} // namespace

#endif // ENABLE_V4L2SRC

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
