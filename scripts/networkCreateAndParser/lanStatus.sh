#!/bin/bash
sudo nmap -sP 169.254.12.2-254 | grep 169.254.12 > /home/pi/Desktop/network
g++ parseaNet.cpp -o parser
./parser
cat network
rm parser