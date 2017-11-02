Creación y lanzamiendo de servidor NFS como un daemon del sistema:

1 Crear el scritp ::::::::::::::::::::::::::::::::::::::::::::::::

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

El orden de estos últimos comandos es esencial

2 Crear el daemon ::::::::::::::::::::::::::::::::::::::::::::::::

sudo chmod +x rickStart.sh
sudo cp rickStart.sh /etc/init.d/
sudo cd /etc/init.d
sudo update-rc.d rickStart.sh defaults

3 Errores ::::::::::::::::::::::::::::::::::::::::::::::::::::::::

Ante el error "insserv: warning: script X missing LSB tags and overrides"

SOL: https://dajul.com/2011/01/06/solucion-al-error-insserv-warning-script-x-missing-lsb-tags-and-overrides/

4 Fuentes ::::::::::::::::::::::::::::::::::::::::::::::::::::::::

https://arenlasysadmin.wordpress.com/2013/05/05/ejecutar-script-arranque-linux/