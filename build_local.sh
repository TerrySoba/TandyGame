#!/bin/sh


export WATCOM=`pwd`/compiler
export PATH=$WATCOM/binl:$PATH
export EDPATH=$WATCOM/eddat
export INCLUDE=$WATCOM/h


cd source
wmake -e


