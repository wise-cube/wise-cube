#!/bin/bash

if [ $USER == "di3go" ]; then
  export IP="192.168.1.204"
  source /home/di3go/.sh_env
  make  flash term

fi

if [ $USER == "roberto" ]; then
  export IP="192.168.1.29"
  source /home/roberto/.sh_env
  make  flash term

fi


if [ $USER == "giulai" ]; then
  export IP="192.168.1.139"
  source /home/giulai/.sh_env
  make  flash term

fi
