FROM ubuntu:18.04

ADD open_watcom.tar.gz .

ENV WATCOM=/opt/opwatcom
ENV PATH=$WATCOM/binl:$PATH
ENV EDPATH=$WATCOM/eddat
ENV INCLUDE=$WATCOM/h

CMD cd /build && wmake

