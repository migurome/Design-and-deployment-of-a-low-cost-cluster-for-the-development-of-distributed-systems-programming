#!/bin/bash
### BEGIN INIT INFO
# Provides:		M.Romero
# Required-start:	$syslog
# Required-stop:	$syslog
# Default-Start:	2 3 4 5
# Default-Stop:		0 1 6
# Short-Description:	Inicializacion de servicios nfs
# Description:
#
### END INIT INFO

sudo exportfs -a
sudo /etc/init.d/rpcbind restart
sudo /etc/init.d/nfs-kernel-server restart