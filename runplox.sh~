
#m_dir = /usr/realtime/modules
#mods = "hal lxrt sem mbx msg fifos tbx bits mq shm"

#for i in $mods do
#	insmod "$m_dir/rtai_$i.ko"
#sync
#done

#sleep 1


sudo rmmod plx_pci sja1000 can_dev can_raw can
sudo mknod --mode=a+rw /dev/can0 c 54 0
sync
sudo insmod ~/Documents/CAN/esdcan-pci200-linux-2.6.x-x86-3.9.2/src/esdcan-pci200.ko
