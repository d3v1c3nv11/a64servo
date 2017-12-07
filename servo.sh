#!/bin/bash

#check for root priv
if [ "$(id -u)" != "0" ]; then
        echo -ne "This script must be executed as root. Exiting\n" >&2
        exit 1
fi

SUDOUSER=$(who am i | awk '{print $1}')

if [ "$1" = "load" ];then

if lsmod | grep a64servo &> /dev/null ; then
  echo "Module already loaded! Exiting..."
  exit 0
else
	insmod a64servo.ko
	echo "Module loaded"
fi
		
if ls /dev | grep servo &> /dev/null ; then
  echo "Node existed! Exiting..."
  exit 0
else
        mknod /dev/servo c 215 0
        chgrp servo /dev/servo
        chmod 665 /dev/servo

        if cat /etc/group | grep servo &> /dev/null; then
	echo Group servo exist. Skipping next...
	else
	groupadd servo
	echo Group servo added. Adding $SUDOUSER to group
        fi
	if id -Gn $SUDOUSER | grep servo &> /dev/null; then
	echo $SUDOUSER already member to group servo
	else 
	usermod -a -G servo $SUDOUSER
	echo $SUDOUSER added to group servo
	echo Loging again as $SUDOUSER
	echo exec su -l $SUDOUSER
	exec su -l $SUDOUSER
	fi
	echo "Node created"
fi
elif [ "$1" = "unload" ];then
if lsmod | grep a64servo &> /dev/null ; then
	rmmod a64servo.ko
	echo "Module unloaded"
fi
if ls /dev | grep servo &> /dev/null ; then
	rm /dev/servo
	echo "Node deleted"
fi
else
	echo "usage: servo (un)load"
fi
