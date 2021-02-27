#! /bin/bash
IFACE="eno1"
TAP="tap-ws"
if [[ -z `ifconfig | grep $TAP` ]]
then
	echo "Tap interface not found, creating"
	# sudo $RIOTBASE/dist/tools/tapsetup/tapsetup 
	sudo ip tuntap add $TAP mode tap 
	sudo brctl addif bridge-ws $TAP
	sudo sysctl -w net.ipv6.conf.$TAP.forwarding=0
    sudo sysctl -w net.ipv6.conf.$TAP.accept_ra=1
	sudo ip link set $TAP up

fi
BUILD_IN_DOCKER=1  BOARD=native PORT=$TAP make all term
sudo ip link del $TAP 


