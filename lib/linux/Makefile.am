## lib/Makefile.am

include $(top_srcdir)/Makefile.common

lib_LTLIBRARIES   = libopenvideo.la
INCLUDES =  -I$(top_srcdir)/src  -I$(TINYXML_INCLUDE) 

ov_includedir=${prefix}/include/openvideo
ov_include_HEADERS= \
	$(top_srcdir)/src/openVideo.h \
	$(top_srcdir)/src/configOV.h

core_includedir = ${prefix}/include/openvideo/core
core_include_HEADERS= \
	$(top_srcdir)/src/core/Manager.h \
	$(top_srcdir)/src/core/State.h \
	$(top_srcdir)/src/core/NodeFactory.h \
	$(top_srcdir)/src/core/Node.h  \
	$(top_srcdir)/src/core/Timer.h \
	$(top_srcdir)/src/core/TimerHandler.h \
	$(top_srcdir)/src/core/PixelFormat.h 

testSrc_includedir = ${prefix}/include/openvideo/nodes/TestSrc
testSrc_include_HEADERS= \
	$(top_srcdir)/src/nodes/TestSrc/TestSrcFactory.h \
	$(top_srcdir)/src/nodes/TestSrc/TestSrc.h

glutsink_includedir = ${prefix}/include/openvideo/nodes/GLUTSink
glutsink_include_HEADERS= \
	$(top_srcdir)/src/nodes/GLUTSink/GLUTSinkFactory.h \
	$(top_srcdir)/src/nodes/GLUTSink/GLUTSink.h

gl_TEXTURE_2D_Sink_includedir = ${prefix}/include/openvideo/nodes/GL_TEXTURE_2D_Sink
gl_TEXTURE_2D_Sink_include_HEADERS= \
	$(top_srcdir)/src/nodes/GL_TEXTURE_2D_Sink/GL_TEXTURE_2D_SinkFactory.h \
	$(top_srcdir)/src/nodes/GL_TEXTURE_2D_Sink/GL_TEXTURE_2D_Sink.h 


libopenvideo_la_SOURCES = $(top_srcdir)/src/core/Manager.cxx \
			$(top_srcdir)/src/core/State.cxx \
			$(top_srcdir)/src/core/NodeFactory.cxx \
			$(top_srcdir)/src/core/Node.cxx \
			$(top_srcdir)/src/core/Timer.cxx \
			$(top_srcdir)/src/core/TimerHandler.cxx \
			$(top_srcdir)/src/core/PixelFormat.cxx \
			$(top_srcdir)/src/nodes/TestSrc/TestSrcFactory.cxx \
			$(top_srcdir)/src/nodes/TestSrc/TestSrc.cxx \
			$(top_srcdir)/src/nodes/GLUTSink/GLUTSinkFactory.cxx \
			$(top_srcdir)/src/nodes/GLUTSink/GLUTSink.cxx \
			$(top_srcdir)/src/nodes/GL_TEXTURE_2D_Sink/GL_TEXTURE_2D_SinkFactory.cxx \
			$(top_srcdir)/src/nodes/GL_TEXTURE_2D_Sink/GL_TEXTURE_2D_Sink.cxx  

libopenvideo_la_LDFLAGS = -L/usr/local/lib 
libopenvideo_la_LIBADD   =  -lACE -lGL  -ltinyxml -lGLU -lglut 
