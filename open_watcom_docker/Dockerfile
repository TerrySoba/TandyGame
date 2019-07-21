FROM alpine:3.10.1

RUN apk add --update \
    make \
    nasm \
    upx \
  && rm -rf /var/cache/apk/*

ADD open_watcom.tar.gz .

ENV WATCOM=/opt/opwatcom
ENV PATH=$WATCOM/binl:$PATH
ENV EDPATH=$WATCOM/eddat
ENV INCLUDE=$WATCOM/h

CMD cd /build && make
