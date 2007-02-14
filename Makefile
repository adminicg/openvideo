all:
	scons --no-cache ENABLE_V4L2SRC=1 ENABLE_GLUTSINK=1 ENABLE_FFMPEGSRC=1 ENABLE_SWITCH=1 PREFIX=/usr/local

install:
	scons install

clean:
	scons -c
