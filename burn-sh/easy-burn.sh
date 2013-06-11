#!/bin/bash

EXPECTED_ARGS=1
if [ $# == $EXPECTED_ARGS ]
then
    IMG_PATH=pwd
    echo "Assuming Default Locations for Prebuilt Images"
    # need to allow f03ref*-ota-eng.* for all different product configuration
    $0 $1 ${IMG_PATH}/$2 ${IMG_PATH}/$3 ${IMG_PATH}/$4
	exit
fi

if [[ -z $1 || -z $2 || -z $3 || -z $4 ]]
then
	echo "  burning Usage:"
	echo "  Example: ./easy-burn.sh inode uboot uImage rootfs"
	exit
fi

if ! [[ -e $2 ]]
then
	echo "Incorrect u-boot-aml-ucl.bin location!"
	exit
fi

if ! [[ -e $3 ]]
then
	echo "Incorrect uImage location!"
	exit
fi

if ! [[ -e $4 ]]
then
	echo "Incorrect rootfs location!"
	exit
fi

echo "All data on "$1" now will be destroyed! Continue? [y/n]"
read ans
if ! [ $ans == 'y' ]
then
	exit
fi

STORAGE_CHECK=`sudo fdisk -l $1 | grep $1`
echo "$STORAGE_CHECK"
if test "$STORAGE_CHECK" = ""; then
    echo "Incorrect device node!"
    exit
fi

echo "[Unmounting all existing partitions on the device ]"

sudo umount $1*

echo "[Partitioning $1...]"

DRIVE=$1
sudo dd if=/dev/zero of=$DRIVE bs=1024 count=1024 &>/dev/null

echo "[Making u-boot $2...]"
sudo dd if=$2 of=$DRIVE bs=512 &>/dev/null

SIZE=`sudo fdisk -l $DRIVE | grep Disk | awk '{print $5}'`
	 
echo DISK SIZE - $SIZE bytes
 
CYLINDERS=`echo $SIZE/255/63/512 | bc`
 
echo CYLINDERS - $CYLINDERS
{
echo 1,,0x0b,*
} | sudo sfdisk -D -H 255 -S 63 -C $CYLINDERS $DRIVE &> /dev/null

INTERFACE=`ls -l $1 | cut -d '/' -f3 | cut -c 1-6`
if [ $INTERFACE == 'mmcblk' ]; then
    POSTFIX='p'
else
    POSTFIX=''
fi

echo "[Making filesystems...]"

sudo mkfs.msdos -n recovery "$1""$POSTFIX"1 &> /dev/null

echo "[Copying kernel image...]"

sudo mount "$1""$POSTFIX"1 /mnt
sudo cp $2 /mnt/u-boot-aml-ucl.bin
sudo cp $3 /mnt/uImage_recovery
sudo cp $4 /mnt/
sudo umount "$1""$POSTFIX"1

echo "[Done]"
exit
