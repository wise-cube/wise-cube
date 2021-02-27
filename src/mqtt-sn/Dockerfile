FROM alpine:latest
RUN apk add --update gcc g++ make perl openssl-dev git
WORKDIR /src
RUN git clone -b master  https://github.com/eclipse/paho.mqtt-sn.embedded-c   
WORKDIR paho.mqtt-sn.embedded-c/MQTTSNGateway       
RUN make SENSORNET=udp6
RUN make install   
RUN make clean    
WORKDIR /src

CMD ["./MQTT-SNGateway",  "-f", "/mqtt/config/paho.conf"]