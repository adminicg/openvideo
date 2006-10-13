#!/usr/bin/env python

import sys
import os
sys.path.append('ICGBuilder')
import icgbuilder
import buildutils

#***************************************************************************************
#
#                  BUILD TARGETS DEFINITION
# This section defines the build targets as dictionaries containing the relevant
# information. The targets could also be loaded from an external file, this is to be
# implemented in the next version of the build system.
# The available fields to provide information about a target are as follows:
#
#   name :      string representing the name of the compiled target without extension.
#   type :      string, depends on the kind of compile, available values are
#                      DL  for dynamic libraries
#                      LIB for static libraries
#                      PRG for consonle applications
#   libs :      list of libraries that will be linked with the target, the builder
#               will attempt to find the appropriate include path, and libpath for
#               these libraries, failure to get this information will result in
#               a script error.
#   use  :      dictionary of other libraries on which the target depends. Each of these
#               will result in the inclusion of one defines flag and the addition of
#               an include path to the compilation. Failure to find include paths
#               for USE variables will result in a script warning
#   src_root:   [Optional] the root directory of the source tree, if missing it
#               will be set by the ConfigurationBuilder
#   src_ignore: [Optional] list of directories or files inside the src_root tree
#               that must be ignored for this target
#   src_use:    [Optional] list of directories or files inside the src_root tree
#               that must be used to build the target. If not defined, all folders
#               and files inside the src_root will be used to build the target
#
#***************************************************************************************

version      = '1.0.0'
project      = 'OpenVideo'
description  = project + ' is a modified small library for handling video input'
mainlib      = 'openvideo'
installRoot  = os.getcwd()
includeDir   = os.path.join(os.getcwd(),'include')
prefix       = ''

enableFlags = { 'VIDEOWRAPPERSRC'    :    'false',
                'SPECTECSRC'         :    'false',
                'GLUTSINK'           :    'true',
                'GL_TEXTURE_2D_SINK' :    'true',
                'TESTSRC'            :    'true',
                'V4LSRC'             :    'false',
                'V4L2SRC'            :    'false',
                'LIVE555SRC'         :    'false',
                'FFMPEGSRC'          :    'false',
                'FFMPEGSINK'         :    'false',
                'VIDEOSINK'          :    'true',
                'DSVLSRC'            :    'true',
                'OPENCV'             :    'false'}



# list of libraries that will be searched by the scanner. The scanner will try to locate the libraries
# and the flags needed to build with those libraries. The obtained result will be used by the build, for
# each target that lists a library in its 'libs' or in its 'use' sections.
libraryList =['ace', 'tinyxml', 'glut', 'openvideo', 'dsvl']

dl ={'name'     : 'openvideo',
    'type'      : 'DL',
    'libs'      : ['ACE', 'TinyXML_Mod', 'glut', 'dsvl'],
    'src_use'   : ['ALL'],
    'defines'   : ['"OPENVIDEO_DEBUG=0"', '"TINYXML_MOD"']
	}

ov ={'name':'openvideo',
     'type':'PRG',
     'libs':['ACE', 'glut', 'openvideo'],
     'src_use': [os.path.join(os.getcwd(), 'standalone', 'main.cxx')]
     }


gllibs = []
if sys.platform == 'win32':
    dl['src_ignore'] = ['nodes\\v4l.c', 'core\\DLL.cxx']
    gllibs = ['opengl32', 'glu32', 'glut32']
    enableFlags['OPENCV']  = 'false'
    enableFlags['DSVLSRC'] = 'true'
else:
    gllibs = ['GL', 'GLU', 'glut']
    enableFlags['OPENCV']  = 'true'
    enableFlags['DSVLSRC'] = 'false'

targetList = [dl,ov]
     
#======================== CONFIGURATION SECTION =============================
# Following is the configuration section. In it most of the options for
# building the specified targets will be set, and then written to a file
# called config.user. This file can be later personalized to allow some
# other user defined configuration.
#===========================================================================#

# Following are default flags for creating the supported target types. 
# Default flags will be applied to all targets of the same sort. In this
# section flags for different types of builds in different platforms can be
# set so that they apply to all targets of the same sort. Supported targets
# are dynamic libraries (dllflags), static libraries (libflags), and 
# executables (prgflags).


if sys.platform == 'win32':
	from win32Flags import defaultBuildFlags
	libraryList +=['tinyxml']	
	# list of libraries that might be used by this project
	scannerType = 'env'

	# directories needed for the build and install process

	buildDir     = os.path.join(os.getcwd(), 'build', 'win32')



	# attempt to add the define DLLNAME_EXPORTS to all targets of type DL
	# in most of the cases it might solve the problem of exporting symbols
	for t in targetList:
		if t['type'] == 'DL':
			if t.has_key('defines'):
				t['defines'].append(t['name'].upper() + '_EXPORTS')
			else:
				t['defines'] = [t['name'].upper() + '_EXPORTS']
	
elif sys.platform == 'linux' or sys.platform == 'linux2':
	from linuxFlags import *
	
	scannerType = 'mix'


	# directories needed for the build and install process
	buildDir     = os.path.join(os.getcwd(), 'build', 'linux')



elif sys.platform == 'darwin':
	from darwinFlags import *
	
	# list of libraries that might be used by this project
	scannerType = 'mix'

	buildDir     = os.path.join(os.getcwd(), 'build', 'darwin')


srcDir       = os.path.join(os.getcwd(), 'src')	

config_file='config.opts'
	
print 'CONFIGURE: Searching for installed libraries'
# to get the include path for all libraries we need to retrieve 
# the environment vars pointing to the root of each lib
envvars = os.environ


targets = []
# create the builder with an empty target list
buildConfig = icgbuilder.ConfigBuilder(project, scannerType, envvars,
				       targets, libraryList)

# add extra configuration flags
buildConfig.version  = version
buildConfig.prefix   = prefix
buildConfig.buildDir = buildDir
buildConfig.srcDir   = srcDir
buildConfig.desc     = description
buildConfig.mainlib  = mainlib
# setup the default build flags in the configurator
buildConfig.setDefaultBuildFlags(defaultBuildFlags)

# add main include directory to all configurations
buildConfig.setIncDir(includeDir)

# add enable flags
for eflag in enableFlags:
	buildConfig._enable[eflag] = enableFlags[eflag]

# create the build targets
buildConfig.setTargetList(targetList)

# this call makes all the targets try to guess their include and library flags
# and prepares the configuration for the build
buildConfig.createBuildEnvironments()
# write a config file to be read by scons and used to build each target
buildConfig.writeConfigFile(config_file)

#-----------------------------------------------------------------------------
# Read the options from the config file and update the various necessary flags
# 
#
#-----------------------------------------------------------------------------


list_opts = []
user_options = Options (config_file, ARGUMENTS)
user_options_env = Environment (ENV = os.environ, options = user_options)

#win32 options
optlist = buildConfig.generateOptionList()
for opt in optlist:
	user_options.Add(opt)
	
uselist = buildConfig.generateUseList()
for use in uselist:
	user_options.Add(BoolOption(use[0], use[1], use[2]))

enablelist = buildConfig.generateEnableList()
for enable in enablelist:
	user_options.Add(BoolOption (enable[0], enable[1], enable[2]))

user_options.Add(EnumOption('BUILD_BINARY', buildConfig.buildType,'Select a release or debug binary.', allowed_values = ('release', 'debug', 'custom')))
	
#==========================================================================
# Intermediate configuration. The config file, config.opts is autogenerated
# using the options from config.user (is it necessasry to have 2 of them
#                                     ????? Eduardo)
#==========================================================================


user_options.Update (user_options_env)
user_options.Save('config.opts', user_options_env)
user_options_dict = user_options_env.Dictionary()
Help(user_options.GenerateHelpText(user_options_env))
env = Environment (ENV = os.environ)
ibuilder = icgbuilder.IcgBuilder(user_options_dict, env)

conf = Configure(env)
#****************************************************************************
# Add defines if using a set of nodes
#****************************************************************************

if user_options_dict['ENABLE_VIDEOWRAPPERSRC']:
    env.AppendUnique(CPPDEFINES = ['ENABLE_VIDEOWRAPPERSRC'])

if user_options_dict['ENABLE_SPECTECSRC']:
    env.AppendUnique(CPPDEFINES = ['ENABLE_SPECTECSRC'])
    
if user_options_dict['ENABLE_GLUTSINK']:
    env.AppendUnique(CPPDEFINES = ['ENABLE_GLUTSINK'])
#    gllibs.append ('glut')
    env.AppendUnique(LIBS = gllibs)
    
if user_options_dict['ENABLE_GL_TEXTURE_2D_SINK']:
    env.AppendUnique(CPPDEFINES = ['ENABLE_GL_TEXTURE_2D_SINK'])
    env.AppendUnique(LIBS = gllibs)
    
if user_options_dict['ENABLE_TESTSRC']:
    env.AppendUnique(CPPDEFINES = ['ENABLE_TESTSRC'])
    
if user_options_dict['ENABLE_V4LSRC']:
    env.AppendUnique(CPPDEFINES = ['ENABLE_V4LSRC'])

if user_options_dict['ENABLE_V4L2SRC']:
    env.AppendUnique(CPPDEFINES = ['ENABLE_V4L2SRC'])
#    env.AppendUnique(CCFLAGS = ['-g'])

if user_options_dict['ENABLE_LIVE555SRC']:
    env.AppendUnique(CPPDEFINES = ['ENABLE_LIVE555SRC'])

if user_options_dict['ENABLE_FFMPEGSINK']:
    env.ParseConfig('ffmpeg-config --cflags --libs avformat')
    env.AppendUnique(CPPDEFINES = ['ENABLE_FFMPEGSINK'])

if user_options_dict['ENABLE_FFMPEGSRC']:
    env.ParseConfig('ffmpeg-config --cflags --libs avformat')
    env.AppendUnique(CCFLAGS = ['-g'])
    env.AppendUnique(CPPDEFINES = ['ENABLE_FFMPEGSRC'])

if user_options_dict['ENABLE_VIDEOSINK']:
    env.AppendUnique(CPPDEFINES = ['ENABLE_VIDEOSINK'])

if user_options_dict['ENABLE_DSVLSRC']:
    env.AppendUnique(CPPDEFINES = ['ENABLE_DSVLSRC'])

if user_options_dict['ENABLE_OPENCV']:
    if conf.TryAction('pkg-config --exists opencv')[0]:
        env.ParseConfig('pkg-config --cflags --libs opencv')
        env.AppendUnique(CPPDEFINES = ['ENABLE_OPENCV'])
    else:
        print "****************************************************************************"
        print "WARNING: You need to have OpenCV installed and opencv.pc in"
        print "your PKG_CONFIG_PATH in order for OpenCV stuff to work."
        print "****************************************************************************"

conf.Finish()


print "\n"
print "============================================================"
print "=      Configuration options for "+ project +" compile       ="
print "============================================================"

#==================    BUILD SECTION    ====================================
#
# In this section, the targets are compiled using the options from 
# config.opts, which are derived themselves from those in config.user
#
#===========================================================================


ibuilder.buildAll()
ibuilder.installAll()

###########################################################################
# Done:)
###########################################################################

