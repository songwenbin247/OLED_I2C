#!/bin/bash

modprobe i2c-dev
IPs_old=" "
No_ip=0
oled ON
oled CLS
while [ 1 ] 
do 
	IPs=`hostname -I`
	if [ ! -z "$IPs" ]; then
	        No_ip=0;
		if [ "$IPs" != "$IPs_old" ]; then
			IPv=(${IPs})
			IPn=${#IPv[@]} 
			if [ $IPn -gt 3 ]; then 
				txt=1
			else
				txt=2
			fi
			oled ON
			oled CLS
			oled 0 0 "Availiable Ip:" $txt
			
			for ((i=0; i<$IPn; i++)) {
				oled 0 $((($i + 1) * $txt)) ${IPv[$i]}  $txt
			}
			IPs_old=$IPs
		fi
	else
		if [ $No_ip -eq 0 ]; then
			oled ON
			oled CLS
			oled 0 2 "No Ip:" 2 
			No_ip=1
		fi
        fi
	sleep 10
done




