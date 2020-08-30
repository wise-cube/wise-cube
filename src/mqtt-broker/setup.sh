#!/bin/bash
curl https://mosquitto.org/files/source/mosquitto-1.6.10.tar.gz > mosquitto.tar.gz
tar -xf mosquitto.tar.gz 
rm mosquitto.tar.gz 
mv mosquitto-1.6.10 src
(mkdir build
cd build 
cmake -DWITH_WEBSOCKETS=ON ../src 
make  
mv src bin
mv bin/mosquitto ../
)

