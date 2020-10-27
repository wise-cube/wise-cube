#!/bin/bash

if [ $USER == "di3go" ]; then
  export BROKER_HOST="fe80::677d:9f78:d931:2e26"
  source /home/di3go/.sh_env
  make  flash term

fi

if [ $USER == "roberto" ]; then
  export BROKER_HOST="192.168.1.29"
  source /home/roberto/.sh_env
  make  flash term

fi


if [ $USER == "giulai" ]; then
  export BROKER_HOST="192.168.1.139"
  source /home/giulai/.sh_env
  make  flash term

fi
