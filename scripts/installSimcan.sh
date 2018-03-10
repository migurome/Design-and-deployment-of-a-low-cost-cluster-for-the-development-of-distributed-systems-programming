#!/bin/bash

#    ___           _        _            _                   _        ____ ___ __  __  ____    _    _   _       _     
#   |_ _|_ __  ___| |_ __ _| | __ _  ___(_) ___  _ __     __| | ___  / ___|_ _|  \/  |/ ___|  / \  | \ | |  ___| |__  
#    | || '_ \/ __| __/ _` | |/ _` |/ __| |/ _ \| '_ \   / _` |/ _ \ \___ \| || |\/| | |     / _ \ |  \| | / __| '_ \ 
#    | || | | \__ \ || (_| | | (_| | (__| | (_) | | | | | (_| |  __/  ___) | || |  | | |___ / ___ \| |\  |_\__ \ | | |
#   |___|_| |_|___/\__\__,_|_|\__,_|\___|_|\___/|_| |_|  \__,_|\___| |____/___|_|  |_|\____/_/   \_\_| \_(_)___/_| |_|
#   

mv omnetpp-4.6-src.tgz ~/omnetpp-4.6-src.tgz
mv inet-2.6-src.tgz ~/inet-2.6-src.tgz
mv simcan.tar ~/simcan.tar
cd ~

echo "  ___                       _   "
echo " / _ \ _ __ ___  _ __   ___| |_ "
echo "| | | | '_   _ \| '_ \ / _ \ __|"
echo "| |_| | | | | | | | | |  __/ |_ "
echo " \___/|_| |_| |_|_| |_|\___|\__|"



sleep 5


tar -xzvf omnetpp-4.6-src.tgz
sudo apt-get update
sudo apt-get install build-essential gcc g++ bison flex perl  tcl-dev tk-dev libxml2-dev zlib1g-dev default-jre doxygen graphviz libwebkitgtk-1.0-0 openmpi-bin libopenmpi-dev libpcap-dev
sudo apt-get install gnome-color-chooser

cd omnetpp-4.6
sudo . setenv
./configure
make -j 4


##################################################
clear
echo " ___            _   "
echo "|_ _|_ __   ___| |_ "
echo " | || '_ \ / _ \ __|"
echo " | || | | |  __/ |_ "
echo "|___|_| |_|\___|\__|"

sleep 5

cd ~
tar -xzvf inet-2.6-src.tgz
mkdir omnetpp-4.6/proyects
mv inet omnetpp-4.6/proyects/
cd omnetpp-4.6/proyects/inet
sudo apt-get install libavcodec-dev libavformat-dev
make makefiles
make -j 4

##################################################
clear

echo " *      ____  _                           "
echo " *     / ___|(_)_ __ ___   ___ __ _ _ __  "
echo " *     \___ \| | '_   _ \ / __/ _  | '_ \ "
echo " *      ___) | | | | | | | (_| (_| | | | |"
echo " *     |____/|_|_| |_| |_|\___\__,_|_| |_|"


sleep 5
cd ~
tar -xvf simcan.tar
mv simcan omnetpp-4.6/proyects/
cd omnetpp-4.6/proyects/simcan
export omnetpp_root=$HOME/pi/omnnetpp-4.6
export INET_HOME=$omnetpp_root/projects/inet
export SIMCAN_HOME=$omnetpp_root/projects/simcan
export LD_LIBRARY_PATH=$omnetpp_root/lib:$LD_LIBRARY_PATH 
export PATH=omnetpp_root/bin:$PATH
make makefiles
make -j 4


