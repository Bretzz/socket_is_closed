#!/bin/bash

IP=$(ipconfig getifaddr en0)

export IP
#env | grep IP
echo $IP
./client $1
#sed -i "s/IP/$IP_X/g" ClientUDP.c