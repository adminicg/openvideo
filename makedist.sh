#!/bin/sh

NAME=openvideo-1.0.0

rm -f ../$NAME
ln -s `pwd` ../$NAME
pushd .. > /dev/null
tar cfjh $NAME.tar.bz2 \
--exclude "*.bz2" \
--exclude "*.dll" \
--exclude "*.gz" \
--exclude "*.o" \
--exclude "*.old*" \
--exclude "*.os" \
--exclude "*.pyc" \
--exclude "*.so*" \
--exclude "*.vcproj" \
--exclude "*.zip" \
--exclude "*~" \
--exclude ".sconf_temp" \
--exclude ".sconsign" \
--exclude ".sconsign*" \
--exclude ".svn" \
--exclude "Makefile*" \
--exclude "build.opts" \
--exclude "config.*" \
$NAME
popd > /dev/null
mv ../$NAME.tar.bz2 .
rm ../$NAME
