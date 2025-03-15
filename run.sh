#!/bin/bash

if [ "`uname`" = "Linux" ]; then
	LOCAL_IP=$(hostname -i);
else
	LOCAL_IP=$(ipconfig getifaddr en0);
fi
#IP=$(ipconfig getifaddr en0 || hostname -i)

export LOCAL_IP SERVER_IP=$1 NAME=$2
env | grep _IP=
env | grep ^NAME=
make  && ./game
#sed -i "s/IP/$IP_X/g" ClientUDP.c