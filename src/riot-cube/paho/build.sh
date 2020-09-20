#!/bin/sh

if [ $USER == "di3go" ]; then
  export IP="192.168.1.204"
  source /home/di3go/.sh_env
  make  flash term

fi

if [ $USER == "roberto" ]; then
  export IP="172.20.10.4"
  source /home/roberto/.sh_env
  make  flash term

fi