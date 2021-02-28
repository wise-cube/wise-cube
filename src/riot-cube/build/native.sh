#! /bin/sh
export IFACE=` route | grep '^default' | grep -o '[^ ]*$'`
export TAP="tap-ws"
export BRIDGE="bridge-ws-ext"


# sudo $RIOTBASE/dist/tools/tapsetup/tapsetup 
# sudo ip tuntap add $TAP mode tap 
# sudo brctl addif bridge-ws $TAP
# sudo sysctl -w net.ipv6.conf.$TAP.forwarding=0
# sudo sysctl -w net.ipv6.conf.$TAP.accept_ra=1
# sudo ip link set $TAP up
if [ "0" = "$#" ]
then 
    echo hola
    sudo $RIOTBASE/dist/tools/tapsetup/tapsetup -c 3 -t $TAP -b $BRIDGE -f
    BUILD_IN_DOCKER=1  BOARD=native PORT=${TAP}1 make all term  

else
    case "$@" in
        
        -c)  sudo "$RIOTBASE/dist/tools/tapsetup/tapsetup" -b "$BRIDGE" -f -d
            BUILD_IN_DOCKER=1  BOARD=native make clean
            break ;;

        -r)  
            BUILD_IN_DOCKER=1  BOARD=native make term
            break ;;

        *)  echo "unrecognized flag"

    esac

fi





