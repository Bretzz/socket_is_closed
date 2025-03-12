#!/bin/bash

if [ "`uname`" = "Linux" ]; then
	LOCAL_IP=$(hostname -i);
else
	LOCAL_IP=$(ipconfig getifaddr en0);
fi
#IP=$(ipconfig getifaddr en0 || hostname -i)

export LOCAL_IP SERVER_IP=$1
env | grep _IP=
make  && ./game
#sed -i "s/IP/$IP_X/g" ClientUDP.c