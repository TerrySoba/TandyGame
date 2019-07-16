#!/bin/sh

docker run -v `pwd`/source/:/build open_watcom &&
rm -rf release && mkdir release &&
cat source/distfiles.txt | xargs -I FILENAME cp source/FILENAME release

