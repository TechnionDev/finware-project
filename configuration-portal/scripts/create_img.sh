#!/bin/bash -v
# Install pishrink
wget https://raw.githubusercontent.com/Drewsif/PiShrink/master/pishrink.sh
chmod +x pishrink.sh
mv pishrink.sh /usr/local/bin

# Create image
dd if=/dev/mmcblk0 of=/media/icst/MOUNT_POINT/myimg.img bs=1M
cd /media/icst/MOUNT_POINT
pishrink.sh -z myimg.img

