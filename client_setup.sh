#!/bin/bash

IP_D=$(hostname -i)

ip_to_hex() {
  printf '%02x' ${1//./ }
}

IP_X=0x`ip_to_hex $IP_D`

sed -i "s/IP/$IP_X/g" ClientUDP.c
