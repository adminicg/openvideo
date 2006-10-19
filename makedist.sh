#!/bin/sh

NAME=openvideo-1.0.0

rm -f ../$NAME
ln -s `pwd` ../$NAME
pushd .. > /dev/null
tar cvfjh $NAME.tar.bz2 \
--exclude "*.dll" \
--exclude "*.pyc" \
--exclude "*.vcproj" \
--exclude "*~" \
--exclude ".svn" \
--exclude "config.*" \
--exclude .sconf_temp \
--exclude .sconsign \
--exclude build.opts \
--exclude $NAME.tar.bz2 \
$NAME
popd > /dev/null
mv ../$NAME.tar.bz2 .
rm ../$NAME
