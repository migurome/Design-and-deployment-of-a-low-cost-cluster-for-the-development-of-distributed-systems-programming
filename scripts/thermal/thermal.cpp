#include <fstream>
#include <iostream>
#include <string>
#include <stdlib.h>

using namespace std;

int main() {

	fstream fichero;
	string mydata;
	
	fichero.open("/etc/hostname");
	
	if(fichero.is_open()){ 
		fichero >> mydata;		
		fichero.close();
		
		fichero.open("/home/pi/sys_log_thermal");
	
		if(fichero.is_open()){
			fichero.seekg(0,fichero.end);
			fichero << mydata << " ";
			fichero.close();
		}
	
		system("cat /sys/class/thermal/thermal_zone0/temp >> /home/pi/sys_log_thermal");
	}
	else{
		cout << "Try again with 'sudo' ..." << endl;
		return -1;
	}
	
	return 0;
}