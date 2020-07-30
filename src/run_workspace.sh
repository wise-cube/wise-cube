#!/bin/sh
# Session Name
session="WiseCube"


tmux new-session \; \
  send-keys 'zsh' C-m \;
  split-window -v \; \
  send-keys '( cd ./cube-riot/ && ./run.sh )' C-m \; \
  split-window -h -p 30 \; \
  send-keys '( cd ./mqtt/ && ./run.sh )' C-m \; \
  split-window -h  \; \
  send-keys '( cd ./http-server/ && ./run.sh )' C-m \; \

#   split-window -h -p 30 \; \
#   select-pane -t 1 \; \
#  split-window -v \; \
  
  

