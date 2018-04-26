#     ____                     _   __                                                                        
#    / ___|_ __ ___  __ _  ___(_) /_/  _ __    _   _                                                         
#   | |   | '__/ _ \/ _` |/ __| |/ _ \| '_ \  | | | |                                                        
#   | |___| | |  __/ (_| | (__| | (_) | | | | | |_| |                                                        
#    \____|_|  \___|\__,_|\___|_|\___/|_| |_|  \__, |  _             _                                       
#   | | __ _ _ __  ______ _ _ __ ___ (_) ___ _ |___/__| | ___     __| | ___                                  
#   | |/ _` | '_ \|_  / _` | '_ ` _ \| |/ _ \ '_ \ / _` |/ _ \   / _` |/ _ \                                 
#   | | (_| | | | |/ / (_| | | | | | | |  __/ | | | (_| | (_) | | (_| |  __/                                 
#   |_|\__,_|_| |_/___\__,_|_| |_| |_|_|\___|_| |_|\__,_|\___/   \__,_|\___|                                 
#                        _     _              _   _ _____ ____                                               
#    ___  ___ _ ____   _(_) __| | ___  _ __  | \ | |  ___/ ___|    ___ ___  _ __ ___   ___                   
#   / __|/ _ \ '__\ \ / / |/ _` |/ _ \| '__| |  \| | |_  \___ \   / __/ _ \| '_ ` _ \ / _ \                  
#   \__ \  __/ |   \ V /| | (_| | (_) | |    | |\  |  _|  ___) | | (_| (_) | | | | | | (_) |                 
#   |___/\___|_|    \_/_|_|\__,_|\___/|_|    |_| \_|_|   |____/ _ \___\___/|_| |_| |_|\___/                  
#    _   _ _ __     __| | __ _  ___ _ __ ___   ___  _ __     __| | ___| |  ___(_)___| |_ ___ _ __ ___   __ _ 
#   | | | | '_ \   / _` |/ _` |/ _ \ '_ ` _ \ / _ \| '_ \   / _` |/ _ \ | / __| / __| __/ _ \ '_ ` _ \ / _` |
#   | |_| | | | | | (_| | (_| |  __/ | | | | | (_) | | | | | (_| |  __/ | \__ \ \__ \ ||  __/ | | | | | (_| |
#    \__,_|_| |_|  \__,_|\__,_|\___|_| |_| |_|\___/|_| |_|  \__,_|\___|_| |___/_|___/\__\___|_| |_| |_|\__,_|
#       

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