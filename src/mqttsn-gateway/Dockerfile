FROM alpine:latest
WORKDIR /build
RUN apk add --update make git zsh openssl-dev g++
RUN git clone -b develop https://github.com/eclipse/paho.mqtt-sn.embedded-c   
WORKDIR /build/paho.mqtt-sn.embedded-c/MQTTSNGateway       
RUN make SENSORNET=udp6
RUN make install   
RUN make clean  
CMD [ "/build/MQTT-SNGateway", "-f", "/paho/config/gateway.conf" ]
