#!/bin/sh

if [ $USER == "di3go" ]; then
  export IP="192.168.3.200"
  source /home/di3go/.sh_env
  make  flash term
#  if [ "$?" != "0" ]; then
#      sudo chown di3go /dev/ttyUSB0
#      make  flash term
#
#  fi
fi