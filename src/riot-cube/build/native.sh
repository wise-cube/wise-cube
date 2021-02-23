#! /bin/bash
IFACE="eno1"

if [[ -z `ifconfig | grep tap` ]]
then
	echo "Tap interface not found, creating"
	sudo $RIOTBASE/dist/tools/tapsetup/tapsetup 
fi
BUILD_IN_DOCKER=1  BOARD=native make all term

