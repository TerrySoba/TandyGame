#!/bin/bash

# build png to tga converter
mkdir -p Png2Tga/build
pushd Png2Tga/build
cmake ..
make
popd
png2tga=Png2Tga/build/Png2Tga


# now convert png images to tga
for pngfile in images/*.png
do
    directory=`dirname $pngfile`
    filename=`basename $pngfile .png`
    echo Converting image \"${directory}/${filename}.png\" to \"${directory}/${filename}.tga\"
    $png2tga -c ${directory}/${filename}.png ${directory}/${filename}.tga
done

# now convert tiled maps to csv
for tmxfile in levels/*.tmx
do
    directory=`dirname $tmxfile`
    filename=`basename $tmxfile .tmx`
    echo Converting level \"${directory}/${filename}.tmx\" to CSV
    tiled ${directory}/${filename}.tmx --export-map ${directory}/${filename}.csv
done

