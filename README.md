# CSCI3753-PA2
Make modules folder

run sudo mknod -m 777 /dev/pa2_character_device 307 0

run make -C /lib/modules/$(uname -r)/build M=$PWD modules

run sudo insmod pa2_char_device_new.ko

sudo gcc charDriverNew.c -o charDriverNew
