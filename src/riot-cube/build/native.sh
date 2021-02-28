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
    sudo ip link add dev veth-ws0 type veth peer name veth-ws1
    sudo brctl addif bridge-ws-ext veth-ws1
    sudo brctl addif bridge-ws veth-ws0
    sudo ifconfig veth-ws1 up
    sudo ifconfig veth-ws0 up


    BUILD_IN_DOCKER=1  BOARD=native PORT=${TAP}0 make all term  

else
    case "$@" in
        
        -c)  
            BUILD_IN_DOCKER=1  BOARD=native make clean
            sudo ifconfig veth-ws1 down
            sudo ifconfig veth-ws0 down
            sudo brctl delif bridge-ws-ext veth-ws1
            sudo brctl delif bridge-ws veth-ws0

            sudo "$RIOTBASE/dist/tools/tapsetup/tapsetup" -b "$BRIDGE" -f -d
            break ;;

        -r)  
            BUILD_IN_DOCKER=1  BOARD=native PORT=${TAP}1 make term
            break ;;

        *)  echo "unrecognized flag"

    esac

fi





