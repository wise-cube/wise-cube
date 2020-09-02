#! /bin/sh
docker run -it -p 1884:1884 -p 1885:1885 -v `pwd`/conf/:/mosquitto/config/ eclipse-mosquitto /bin/sh
