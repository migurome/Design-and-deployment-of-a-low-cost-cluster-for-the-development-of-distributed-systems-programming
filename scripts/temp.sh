#Version 2,0
while test 0 -eq 0
do
sleep $1
temp=$(/opt/vc/bin/vcgencmd measure_temp | cut -c 6-9) #know the temperature
time=$(date +"%x" ) #dd/mm/yyyy
time2=$(date +"%X") #hh:mm:ss 
echo "$temp"  >> temp.txt
echo "$temp"  

done
exit 0
