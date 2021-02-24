FROM alpine
MAINTAINER Thomas Kerpe <thomas@kerpe.net>

RUN apk --update add git gcc make musl-dev perl
RUN mkdir /tmp/src
WORKDIR  /tmp/src 
RUN git clone  https://github.com/eclipse/mosquitto.rsmb.git
WORKDIR /tmp/src/mosquitto.rsmb/rsmb/src 
RUN make
RUN cp broker_mqtts  /usr/local/bin/

VOLUME ["/mqtt/config"]
COPY rsmb.conf /mqtt/config/


CMD ["broker_mqtts", "/mqtt/config/rsmb.conf"]