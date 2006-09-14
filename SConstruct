#!/usr/bin/env python

import sys
import os
import icgbuilder
import buildutils

#======================== CONFIGURATION SECTION =============================
# Following is the configuration section. In it most of the options for
# building the specified targets will be set, and then written to a file
# called config.user. This file can be later personalized to allow some
# other user defined configuration.
#===========================================================================#


# The targets file contains the definitions for all targets that will be built 
# using this scipt. Read the targets.py file to find out about those options
from targets import targetList, version, project, description, mainlib, installRoot, includeDir, libraryList, enableFlags, gllibs

# Following are default flags for creating the supported target types. 
# Default flags will be applied to all targets of the same sort. In this
# section flags for different types of builds in different platforms can be
# set so that they apply to all targets of the same sort. Supported targets
# are dynamic libraries (dllflags), static libraries (libflags), and 
# executables (prgflags).

dllflags = None
libflags = None
prgflags = None

if sys.platform == 'win32':
	dllflags = { 'cflags'  : ['/O2','/W3', '/EHsc', '/Zi', '/FD', '/MD', '/Gy', '/errorReport:prompt', '/Fd"build\\win32\\vc80.pdb"'],
		     'defines' : ['"WIN32"', '__WIN32__','"NDEBUG"', '"_WINDOWS"', '"_USRDLL"'
				  ,'"_CRT_SECURE_NO_DEPRECATE"', '"_WINDLL"', '"_MBCS"' ],
		     'ldflags' : ['/INCREMENTAL:NO', '/NOLOGO', '/DEBUG', '/SUBSYSTEM:WINDOWS',
				  '/OPT:REF', '/OPT:ICF', '/MACHINE:X86',
				  '/ERRORREPORT:PROMPT','/MANIFEST', '/DLL'],
		     'libs'    : ['kernel32', 'user32', 'gdi32', 'winspool', 'comdlg32',
				  'advapi32', 'shell32', 'ole32', 'oleaut32', 'uuid',
				  'odbc32', 'odbccp32'] 
		     }

	libflags = {'cflags'   : ['/O2','/W3', '/EHsc', '/MD', '/Zi'],
		    'defines'  : ['"WIN32"', '__WIN32__','NDEBUG', '_WINDOWS','_USRDLL',
				  '__x86__', '_CRT_SECURE_NO_DEPRECATE', "_WINDLL", "_MBCS"]
		    
		    }

	prgflags = {'cflags'   : ['/O2', '/Wp64', '/W3', '/FD', '/EHsc', '/MT', '/Zi', '/TP', '/errorReport:prompt'],
		    'defines'  : ['"WIN32"','"NDEBUG"','"_CONSOLE"',"_MBCS"],
		    'ldflags'  : ['/INCREMENTAL:NO', '/NOLOGO','/SUBSYSTEM:CONSOLE',
				  '/MACHINE:X86'],
		    'libs'     : ['odbc32', 'odbccp32', 'kernel32', 'user32', 'gdi32',
				  'winspool', 'comdlg32', 'advapi32', 'shell32', 'ole32'
				  , 'oleaut32', 'uuid','odbc32', 'odbccp32']
		    }

	# list of libraries that might be used by this project
	scannerType = 'env'


	# directories needed for the build and install process
	prefix       = os.getcwd()
	buildDir     = os.path.join(prefix, 'build', 'win32')
	installRoot  = prefix
	installDll   = os.path.join(prefix, 'bin', 'win32')
	installLib   = os.path.join(prefix, 'lib', 'win32')
	installExe   = os.path.join(prefix, 'bin', 'win32')
	srcDir       = os.path.join(prefix, 'src')

	# attempt to add the define DLLNAME_EXPORTS to all targets of type DL
	# in most of the cases it might solve the problem of exporting symbols
	for t in targetList:
		if t['type'] == 'DL':
			if t.has_key('defines'):
				t['defines'].append(t['name'].upper() + '_EXPORTS')
			else:
				t['defines'] = [t['name'].upper() + '_EXPORTS']
	
elif sys.platform == 'linux' or sys.platform == 'linux2':
	defaultflags = {'cflags'  : ['-O2', '-pipe','-Wall'],
			'defines' : ['LINUX']
			}
	dllflags = defaultflags
	libflags = defaultflags
	prgflags = defaultflags
	
	scannerType = 'pkg'
	libraryList =['boost', 'ACE', 'xerces', 'omniORB4', 'omniCOS4', 'omniEvents',
		      'omnithread3','XMLClient']


	# directories needed for the build and install process
	prefix       = os.getcwd()
	buildDir     = os.path.join(prefix, 'build', 'linux')
	installDll   = os.path.join(installRoot, 'lib')
	installLib   = os.path.join(installRoot, 'lib')
	installExe   = os.path.join(installRoot, 'bin')
	srcDir       = os.path.join(prefix, 'src')
	prefix       = ''

elif sys.platform == 'darwin':
	defaultflags = {'cflags'  : ['-O2', '-pipe','-Wall', '-W'],
			'defines' : ['DARWIN']
			}
	dllflags = defaultflags
	libflags = defaultflags
	prgflags = defaultflags
	
	# list of libraries that might be used by this project
	scannerType = 'pkg'
	libraryList =['boost', 'ACE', 'xerces-c', 'x11', 'omniORB4', 'omniCOS4', 'omniEvents',
		      'omnithread3','XMLClient']

	prefix       = os.getcwd()
	buildDir     = os.path.join(prefix, 'build', 'darwin')
	installRoot  = ''
	installDll   = os.path.join(installRoot, 'lib')
	installLib   = os.path.join(installRoot, 'lib')
	installExe   = os.path.join(installRoot, 'bin')
	srcDir       = os.path.join(prefix, 'src')
	prefix = ''

	
defaultBuildFlags = {'DL' : dllflags,
		     'LIB': libflags,
		     'PRG': prgflags}

config_file='config.user'
	
print 'CONFIGURE: Searching for installed libraries'
# to get the include path for all libraries we need to retrieve 
# the environment vars pointing to the root of each lib
envvars = os.environ


targets = []
# create the builder with an empty target list
buildConfig = icgbuilder.ConfigBuilder(project, scannerType, envvars,
				       targets, libraryList)

# add extra configuration flags
buildConfig.version = version
buildConfig.prefix = prefix
buildConfig.buildDir = buildDir
buildConfig.installRootDir = installRoot
buildConfig.installDll     = installDll
buildConfig.installExe     = installExe
buildConfig.installLib     = installLib
buildConfig.srcDir         = srcDir

# setup the default build flags in the configurator
for bfsetup in defaultBuildFlags:
	bf = buildConfig.getBuildFlags(bfsetup)
	bf.copyFromDict(defaultBuildFlags[bfsetup])

# add main include directory to all configurations
buildConfig.addIncDir(includeDir)

# add enable flags
for eflag in enableFlags:
	buildConfig._enable[eflag] = enableFlags[eflag]

# create the build targets
for t in targetList:
	if t.has_key('src_root'):
		srcRoot = t['src_root']
	else:
		srcRoot = None

	tgt = icgbuilder.Target(t['name'], t['type'], srcRoot)
	if t.has_key('use'):
		for u in t['use']:
			tgt.use(u, t['use'][u])
	if t.has_key('libs'):
		for l in t['libs']:
			tgt.link(l)

	if t.has_key('src_use'):
		for u in t['src_use']:
			tgt.useSrc(u)

	if t.has_key('src_ignore'):
		for i in t['src_ignore']:
			tgt.ignoreSrc(i)
	if t.has_key('defines'):
		for d in t['defines']:
			tgt.addDefine(d)

	buildConfig.addTarget(tgt)





# this call makes all the targets try to guess their include and library flags
# and prepares the configuration for the build
buildConfig.createBuildEnvironments()
# write a config file to be read by scons and used to build each target
if not os.path.exists (config_file):
	buildConfig.writeConfigFile(config_file)

#-----------------------------------------------------------------------------
# Read the options from the config file and update the various necessary flags
# 
#
#-----------------------------------------------------------------------------

env = Environment (ENV = os.environ)

if os.environ.has_key('HOME'):
	env.CacheDir(os.path.join(os.environ['HOME'], '.scache'))


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

root_build_dir = user_options_dict['BUILD_DIR']

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

buildType = user_options_dict['BUILD_BINARY']


#list of dynamic libraries to install later
dlls = []
#list of static libraries to install later
libs = []
#list of programs to install later
prgs = []

env.BuildDir(buildConfig.buildDir+ os.sep + 'src', 'src', duplicate=0)
env.BuildDir(buildConfig.buildDir, '.', duplicate=0)


# build all dynamic libraries first
# get the list of dynamic library targets from the user dictionary
dllist = user_options_dict['DLTARGETS']
for target in dllist:
	keyName = 'DL_' + target.upper()
	longkey = keyName + '_' + buildType.upper()
	libbuildir = os.path.join(user_options_dict['BUILD_DIR'], 'lib')
	libraryBuilder = env.Copy()
	libraryBuilder.Append(CCFLAGS = user_options_dict[longkey+'_CCFLAGS'])
	libraryBuilder.Append(CPPPATH = user_options_dict[longkey+'_INCPATH'])
	libraryBuilder.Append(CPPDEFINES = user_options_dict[longkey+'_DEFINES'])
	libraryBuilder.Append(LIBS = user_options_dict[longkey+'_LIBS'])
	libraryBuilder.Append(LIBPATH = user_options_dict[longkey+'_LIBPATH'])
	libraryBuilder.Append(LINKFLAGS = user_options_dict[longkey+'_LDFLAGS'])
	libraryBuilder.SConsignFile (root_build_dir+'scons-signatures')
	
#	libraryBuilder.BuildDir(os.path.join(user_options_dict['BUILD_DIR'],keyName), 'src', duplicate=0)
#	libraryBuilder['OBJPREFIX']= keyName
	libraryBuilder['OBJPREFIX']= os.path.join(user_options_dict['BUILD_DIR'], keyName)
	libraryBuilder['PDB'] = os.path.join(libbuildir, target + '.pdb')
	
	# Print welcoming message that makes you feel warm and fuzzy inside;)
	print "... Building Dynamic Library %s ...\n" %(target.upper())
	
	libs += libraryBuilder.SharedLibrary (os.path.join(libbuildir,target), user_options_dict[keyName+'_SOURCES'] )
	for dll in libs:
#		print "DLL SUFFIX name%s suffix%s\n"%(dll.name, dll.get_suffix())
		if dll.get_suffix() == '.dll':
			manifestname = dll.name + '.manifest'
			manifestname = os.path.join(libbuildir, manifestname)
			#install the dlls to the dynamic library directory
			dlls.append(dll)
			dlls.append(manifestname)
			
			

		
# build all static libraries
# get the list of static libraries from the user dictionary
liblist = user_options_dict['LIBTARGETS']
for target in liblist:
	keyName = 'LIB_' + target.upper()
	longkey = keyName + '_' + buildType.upper()
	libbuildir = os.path.join(user_options_dict['BUILD_DIR'], 'lib')	
	libraryBuilder = env.Copy()
	libraryBuilder.Append(CCFLAGS = user_options_dict[longkey+'_CCFLAGS'])
	libraryBuilder.Append(CPPPATH = user_options_dict[longkey+'_INCPATH'])
	libraryBuilder.Append(CPPDEFINES = user_options_dict[longkey+'_DEFINES'])
	libraryBuilder.Append(LIBS = user_options_dict[longkey+'_LIBS'])
	libraryBuilder.Append(LIBPATH = user_options_dict[longkey+'_LIBPATH'])
	libraryBuilder.Append(LINKFLAGS = user_options_dict[longkey+'_LDFLAGS'])
	libraryBuilder.SConsignFile (root_build_dir+'scons-signatures')

#	libraryBuilder.BuildDir(os.path.join(user_options_dict['BUILD_DIR'],keyName), 'src', duplicate=0)
#	libraryBuilder.BuildDir ('build\\win32\\'+keyName, 'src', duplicate=0)
	libraryBuilder['OBJPREFIX']= os.path.join(user_options_dict['BUILD_DIR'], keyName)
	libraryBuilder['PDB'] = os.path.join(libbuildir, target + '.pdb')
	# Print welcoming message that makes you feel warm and fuzzy inside;)
	print "... Building Static Library %s ...\n" %(target.upper())

	libs += libraryBuilder.Library (os.path.join(libbuildir,target), user_options_dict[keyName+'_SOURCES'] )


# build all programs
# get the list of programs from the user dictionary
prglist = user_options_dict['PRGTARGETS']
for target in prglist:
	keyName = 'PRG_' + target.upper()
	longkey = keyName + '_' + buildType.upper()
	prgbuildir = os.path.join(user_options_dict['BUILD_DIR'], 'bin')	
	prgBuilder = env.Copy()
	prgBuilder.Append(CCFLAGS = user_options_dict[longkey+'_CCFLAGS'])
	prgBuilder.Append(CPPPATH = user_options_dict[longkey+'_INCPATH'])
	prgBuilder.Append(CPPDEFINES = user_options_dict[longkey+'_DEFINES'])
	prgBuilder.Append(LIBS = user_options_dict[longkey+'_LIBS'])
	prgBuilder.Append(LIBPATH = user_options_dict[longkey+'_LIBPATH'])
	prgBuilder.Append(LINKFLAGS = user_options_dict[longkey+'_LDFLAGS'])
	prgBuilder.SConsignFile (root_build_dir+'scons-signatures')

#	libraryBuilder.BuildDir ('build\\win32', 'src', duplicate=0)
        prgBuilder['OBJPREFIX']= os.path.join(user_options_dict['BUILD_DIR'], keyName)
	prgBuilder['PDB'] = os.path.join(prgbuildir, target + '.pdb')
	# Print welcoming message that makes you feel warm and fuzzy inside;)
	print "... Building Static Library %s ...\n" %(target.upper())

	prgs += prgBuilder.Program (os.path.join( prgbuildir,target), user_options_dict[keyName+'_SOURCES'] )
	for bar in prgs:
		s = 'somestring'
#		print "EXE SUFFIX name%s suffix\n" %( dir(bar))
		if type(bar) != type(s) and bar.get_suffix() == '.exe':
			manifestname = bar.name + '.manifest'
			manifestname = os.path.join(prgbuildir, manifestname)
			#install the dlls to the dynamic library directory
			prgs.append(manifestname)




#= INSTALL SECTION =======================================================
#
#  All the targets are installed in this section depending on the target 
#  type, and according to the options specified in the config.opts file.
#
#=========================================================================

## Perform installation in a platform specific manner. For windows
## We have a different kind of folder for different kinds of targets
## although most of the times, dlls will be placed with executables and
## libs in a separate folder.

if sys.platform == 'win32':
	print "WINDOWS!....INSTALLING DLLS in ", user_options_dict['INSTALL_DLL_DIR']
	env.Install(user_options_dict['INSTALL_DLL_DIR'], dlls)
        #install everything under lib
	print "WINDOWS!....INSTALLING LIBS in ", user_options_dict['INSTALL_LIB_DIR']
	env.Install(user_options_dict['INSTALL_LIB_DIR'], libs)
        #install everything under lib
	print "WINDOWS!....INSTALLING EXEs in ", user_options_dict['INSTALL_EXE_DIR']
	env.Install(user_options_dict['INSTALL_EXE_DIR'], prgs)

## Perform installation in a platform specific manner. For Linux, the library version
## must be appended to the library name, and then a symbolic link pointing to the newly
## built library must be added

elif sys.platform == 'linux' or sys.platform == 'linux2' or sys.platform == 'darwin':
	print "LINUX!....."
	include_dir   = includeDir

	include_files = buildutils.listtrav(include_dir, include_dir)
	include_files = buildutils.regexpfilter(include_files, user_options_dict['INCLUDE_FILTER'], True)
	include_files = buildutils.regexpfilter(include_files, user_options_dict['IGNORE_FILTER'], False)


        #****************************************************************************
	# Set project details used in the package-config (.pc) file
	# See tinyxml.pc.in
        #***************************************************************************

	## there is a list of libraries in libs and a list of executables in
	## prgs, those should be installed.
	dlname      = mainlib

	## there must be a way to extract global_defs from the
	## options in the 

	global_defs = []

	env[project.upper() + '_PROJECT_NAME']        = project
	env[project.upper() + '_PROJECT_DESCRIPTION'] = description
	env[project.upper() + '_PROJECT_VERSION']     = version
	env[project.upper() + '_PROJECT_LIBNAME']     = mainlib
	env[project.upper() + '_PREFIX']              = user_options_dict['PREFIX']
	env[project.upper() + '_LIBDIR']              = os.path.join(user_options_dict['INSTALL_LIB_DIR'])
	env[project.upper() + '_PROJECT_DEFINES']     = global_defs
	
	BuildDir(root_build_dir+'/src', 'src', duplicate=0)

	buildutils.appendbuilders(env)
	outname = env.AlwaysBuild(env.Substitute('TinyXMLMod.pc', 'TinyXMLMod.pc.in'))
	# read out library prefix
	lib_prefix = env.subst('$SHLIBPREFIX')
	lib_suffix = env['SHLIBSUFFIX']
	lib_file =  os.path.join('lib', lib_prefix + dlname + lib_suffix)
	print 'LIBFILE WILL BE ',lib_file
	install_dir_name = lambda x:os.path.join(user_options_dict['INSTALL_ROOT_DIR'],
						 user_options_dict['PREFIX'], x)

	install_dir  = {}
	alias_dir    = {}

	for d in ['lib', 'include', os.path.join('lib','pkgconfig')]:
		i_dir = env.Dir(install_dir_name(d))
		print "creating " + str(i_dir) + ' install directory if necessary: ',
		if not i_dir.exists():
			print 'necessary'
			dir_name = str(i_dir)
			env.Execute(Mkdir(dir_name))
			install_dir[d] = i_dir
		else:
			print 'unnecessary'
			install_dir[d] = i_dir

	# Make a copy of the library with the suffix '.2.0.0'

	minor_versions = 3
	if sys.platform != 'darwin':
		target_lib = lib_file + buildutils.get_version_string(version, minor_versions)
	else:
		target_lib = os.path.join('lib', lib_prefix + dlname + buildutils.get_version_string(version, minor_versions) + lib_suffix)


	# This command is doing the same as the one inmediately below
	# Why was it here?
	env.Command(target_lib, lib_file, Copy("$TARGET", "$SOURCE"))
	# Install copy of library in install directory
	print "INSTALL_ROOT_DIR ",user_options_dict['INSTALL_ROOT_DIR']
	print "prefix ",user_options_dict['PREFIX']
	for things in install_dir:
		print "INSTALL_DIR_NAME %s %s "%(install_dir[things],install_dir[things])	

	print 'install_dir ',install_dir['lib']
	print 'target_lib ', target_lib
	print 'lib_file ',lib_file
	lib_inst = env.Install(install_dir['lib'], target_lib)

	# Make symbolic links to the installed copy of the library
	def make_sym_link(target, source, env):
		lncommand = 'ln -s ' + os.path.basename(source[0].abspath) + ' ' + str(target[0])
                #print lncommand
		env.Execute(lncommand)
		return None

	target_lib_symlinks = []

	for i in [i for i in range(0,2) if i!= minor_versions]:
		if sys.platform != 'darwin':
			target_installed_symlink = os.path.join(install_dir_name('lib'), os.path.basename(lib_file) + buildutils.get_version_string(version, i))
		else:
			target_installed_symlink = os.path.join(install_dir_name('lib'),lib_prefix + os.path.basename(dlname) + buildutils.get_version_string(version, i) + lib_suffix)
		
		if not os.path.islink(target_installed_symlink):
			print "target_installed_symlink = ", target_installed_symlink
			target_lib_symlinks += env.Command(target_installed_symlink , lib_inst, make_sym_link)
		else:
			print "existing_symlink = ", target_installed_symlink
	
	cfg_inst = env.Install(install_dir[os.path.join('lib', 'pkgconfig')], outname )
	inc_inst = []
	for include_file in include_files:
		i_file = os.path.join('include', str(include_file))
		inc_inst.append(env.Install(os.path.join(str(install_dir['include']), os.path.dirname(include_file)), i_file))


	icfg = env.Alias('install-config', cfg_inst)
	ilib = env.Alias('install-lib', lib_inst)
	ihdr = env.Alias('install-header', inc_inst)
	symlinks = env.Alias('install-symlinks', target_lib_symlinks)
	env.Alias('install', [ilib, icfg, ihdr, symlinks])

###########################################################################
# Done:)
###########################################################################

