#include <fstream>
#include <string>

using namespace std;


typedef struct {
  string name;
  int temp;
}tDatos;

int main() {


 fstream fichero;
 tDatos mydata;

 fichero.open("/etc/hostname");
 
 if(fichero.is_open()){ 
   
  fichero >> mydata.name;
  
  fichero.close();

 }else
  return -1;

 
 fichero.open("/home/pi/Desktop/sys_log");
 
 if(fichero.is_open()){
  fichero.seekg(0,fichero.end);
  fichero << mydata.name << " " << endl;
 }

 system("cat /sys/class/thermal/thermal_zone0/temp >> /home/pi/Desktop/sys_log")

  return 0;

}