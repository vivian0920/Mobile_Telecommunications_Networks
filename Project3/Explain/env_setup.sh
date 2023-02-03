#!/bin/bash

YELLOW='\033[1;33m'
NC='\033[0m'

set -e
cd ~
sudo apt update -y


# Install OvS 2.11.4

cd ~
sudo apt install gcc make
wget https://www.openvswitch.org/releases/openvswitch-2.11.4.tar.gz
tar zxf openvswitch-2.11.4.tar.gz
cd openvswitch-2.11.4
./configure --prefix=/usr --localstatedir=/var --sysconfdir=/etc
make
sudo make install
# sudo make uninstall
sudo /usr/share/openvswitch/scripts/ovs-ctl start

sudo sed -i "/exit 0$/i /usr/share/openvswitch/scripts/ovs-ctl start" /etc/rc.local

# Finish
echo -e "${YELLOW}*** Installation Finished! ***${NC}"
