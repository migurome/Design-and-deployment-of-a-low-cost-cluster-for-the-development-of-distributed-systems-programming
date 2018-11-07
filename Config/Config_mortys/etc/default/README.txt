#    _   _       _            
#   | \ | | ___ | |_ __ _ ___ 
#   |  \| |/ _ \| __/ _` / __|
#   | |\  | (_) | || (_| \__ \
#   |_| \_|\___/ \__\__,_|___/
#     

Es necesario retardar la carga inicial del sistema para esperar a que el servidor 
esté completamente operativo, para ello una de las opciones es incrementar el tiempo
de espera del grub en los slaves.

Cambio de timeout de GRUB:

Modificar la línea de /etc/default/grub:

GRUB_TIMEOUT=25

Para aplicar los cambios ejecutar:

$update-grub

FUENTES:

https://www.garron.me/es/gnu-linux/definir-windows-arranque-defecto-grub-2.html