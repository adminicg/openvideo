
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

import os

version = '1.0'
project = 'OpenVideo'
installRoot = os.getcwd()


dl ={'name'     : 'openvideo',
    'type'      : 'DL',
    'libs'      : ['ace', 'tinyxml_mod'],
    'src_use'   : ['ALL'],
#    'src_ignore': ['TinyXML/TinyXML_autolink.cpp'],
    'defines'   : ['"OPENVIDEO_DEBUG=0"']
	}


targetList = [dl]

     



