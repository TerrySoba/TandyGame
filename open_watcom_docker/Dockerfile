FROM alpine:3.10

RUN apk add --update \
    nasm \
  && rm -rf /var/cache/apk/*

ADD open_watcom.tar.gz .

ENV WATCOM=/opt/opwatcom
ENV PATH=$WATCOM/binl:$PATH
ENV EDPATH=$WATCOM/eddat
ENV INCLUDE=$WATCOM/h

CMD cd /build && wmake
