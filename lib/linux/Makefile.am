## lib/Makefile.am

include $(top_srcdir)/Makefile.common

lib_LTLIBRARIES   = libopenvideo.la
INCLUDES =  -I$(top_srcdir)/src  -I$(TINYXMLROOT) 
libopenvideo_la_SOURCES = $(top_srcdir)/src/openVideo.h \
			$(top_srcdir)/src/configOV.h \
			$(top_srcdir)/src/core/Manager.h \
			$(top_srcdir)/src/core/Manager.cxx \
			$(top_srcdir)/src/core/Context.h \
			$(top_srcdir)/src/core/Context.cxx \
			$(top_srcdir)/src/core/NodeFactory.h \
			$(top_srcdir)/src/core/NodeFactory.cxx \
			$(top_srcdir)/src/core/Node.h  \
			$(top_srcdir)/src/core/Node.cxx \
			$(top_srcdir)/src/core/Timer.h \
			$(top_srcdir)/src/core/Timer.cxx \
			$(top_srcdir)/src/core/TimerHandle.h \
			$(top_srcdir)/src/core/TimerHandle.cxx \
			$(top_srcdir)/src/nodes/TestSrc/TestSrcFactory.h \
			$(top_srcdir)/src/nodes/TestSrc/TestSrcFactory.cxx \
			$(top_srcdir)/src/nodes/TestSrc/TestSrc.h \
			$(top_srcdir)/src/nodes/TestSrc/TestSrc.cxx \
			$(top_srcdir)/src/nodes/GLUTSink/GLUTSinkFactory.h \
			$(top_srcdir)/src/nodes/GLUTSink/GLUTSinkFactory.cxx \
			$(top_srcdir)/src/nodes/GLUTSink/GLUTSink.h \
			$(top_srcdir)/src/nodes/GLUTSink/GLUTSink.cxx \
			$(top_srcdir)/src/nodes/GL_TEXTURE_2D_Sink/GL_TEXTURE_2D_SinkFactory.h \
			$(top_srcdir)/src/nodes/GL_TEXTURE_2D_Sink/GL_TEXTURE_2D_SinkFactory.cxx \
			$(top_srcdir)/src/nodes/GL_TEXTURE_2D_Sink/GL_TEXTURE_2D_Sink.h \
			$(top_srcdir)/src/nodes/GL_TEXTURE_2D_Sink/GL_TEXTURE_2D_Sink.cxx

libopenvideo_la_LDFLAGS = -L/usr/local/lib 
libopenvideo_la_LIBADD   = -ltinyxml -lACE -lGL -lGLU -lglut
