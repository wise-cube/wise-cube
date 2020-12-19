#!/bin/bash

if [ $USER == "di3go" ]; then
  export SERVER_ADDR="fe80::677d:9f78:d931:2e26"
  export VIRTUAL_SERVER_ADDR="fe80::2428:4fff:fec8:303"
  source /home/di3go/.sh_env
  make  flash term

fi

if [ $USER == "roberto" ]; then
  export SERVER_ADDR="192.168.1.29"
  source /home/roberto/.sh_env
  make  flash term

fi


if [ $USER == "giulai" ]; then
  export SERVER_ADDR="192.168.1.139"
  source /home/giulai/.sh_env
  make  flash term

fi
