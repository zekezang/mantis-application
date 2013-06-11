#!/bin/bash

./adb-tacp push sn_mac_tool /sbin
./adb-tacp shell /sbin/sn_mac_tool

echo 