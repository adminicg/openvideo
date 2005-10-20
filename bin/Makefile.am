## bin/Makefile.am
include $(top_srcdir)/Makefile.common
bin_PROGRAMS = openvideo
INCLUDES =  -I$(top_srcdir)/src 
openvideo_SOURCES = 	$(top_srcdir)/standalone/main.cxx \
			$(top_srcdir)/bin/openVideo.xml
openvideo_LDADD = $(top_srcdir)/lib/libopenvideo.la
