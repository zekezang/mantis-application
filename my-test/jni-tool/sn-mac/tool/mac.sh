#!/bin/bash

./adb-tacp push sn_mac_tool /sbin

./adb-tacp shell /sbin/sn_mac_tool > .tmp_now
if [ -e thai_mac.log ]; then
	tail -n 1 thai_mac.log > .tmp_old
	diff .tmp_now .tmp_old > /dev/null

	if [ "$?" != "0" ]; then
		echo "ok, success done !"
		./adb-tacp shell /sbin/sn_mac_tool >> thai_mac.log
	else
		echo "you have done this mac infomation."
	fi
	
else
	echo "ok, success done !"
	./adb-tacp shell /sbin/sn_mac_tool >> thai_mac.log
fi



