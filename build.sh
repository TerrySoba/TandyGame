#!/bin/sh

BUILD_DATE=`date --rfc-3339=seconds`
echo \#define BUILD_DATE \"$BUILD_DATE\" > source/version.h
GIT_HASH=`git rev-parse HEAD`
echo \#define GIT_HASH \"$GIT_HASH\" >> source/version.h

docker run -v `pwd`/source/:/build open_watcom &&
rm -rf release && mkdir release &&
upx --8086 source/game.exe &&
cat source/distfiles.txt | xargs -I FILENAME cp source/FILENAME release

