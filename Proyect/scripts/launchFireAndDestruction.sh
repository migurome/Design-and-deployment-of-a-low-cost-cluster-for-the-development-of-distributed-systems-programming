#!/bin/bash
#Primer parametro: Numero de core lanzado
 
for i in `seq 1 500`
do
	date -u >> Proceso$1.txt
	echo "Lanzada prueba " $i >> Proceso$1.txt
	./run -u Cmdenv omnetpp_100M.ini -c discoA > salida
done