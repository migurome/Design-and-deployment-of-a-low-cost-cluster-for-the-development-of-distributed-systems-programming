#include <fstream>
#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

using namespace std;

const int NUM_PARAMETROS = 4;
const int HOST_INICIALES = 5;

typedef struct {
  string cadena;
}tElemento;

typedef struct {
  tElemento *lista;
  int num_datos;
  int capacidad;
}tDatos;

bool parser();
void init(tDatos &datos);
void resize(tDatos &datos);
void free(tDatos &datos);

int main() {


	pid_t pid = fork();
	
	if(pid == 0){
		system("sudo nmap -sP 169.254.12.2-254 | grep 169.254.12 > /home/pi/Desktop/network");
	}else{
	
		wait(NULL);

		if(!parser())
			cout << "Error en la creacion del fichero" << endl;
	}

	return 0;

}

void init(tDatos &datos) {

  datos.num_datos = 0;
  datos.capacidad = HOST_INICIALES;
  datos.lista = new tElemento[datos.capacidad];

}

void resize(tDatos &datos) {

  tElemento *aux  = new tElemento[datos.num_datos + 1];

  for (int i = 0; i < datos.num_datos; i++)
    aux[i] = datos.lista[i];

  delete[] datos.lista;
  datos.lista = aux;
  datos.capacidad++;
// aux = null;

}

bool parser(){

	ifstream entrada;
	bool apertura = false;
	ofstream salida;
	tDatos misdatos;
	string nada;
	
	entrada.open("/home/pi/Desktop/network");
	
	if (entrada.is_open()) {

		apertura = true;
		
		init(misdatos);
		int t = 0;
		
		while (!entrada.eof()) {
		
			for (int i = 0; i < NUM_PARAMETROS; i++)
				entrada >> nada;
			
			if (misdatos.num_datos == misdatos.capacidad)
				resize(misdatos);
			
			entrada >> misdatos.lista[t].cadena;
			misdatos.num_datos++;
			t++;
		}
	
		entrada.close();
		
		salida.open("/home/pi/Desktop/network");

		if (salida.is_open()) {
		
			for (int i = 0; i < misdatos.num_datos; i++) {
				salida << misdatos.lista[i].cadena;
				if (i < misdatos.num_datos - 1)
					salida << endl;
			}
			
			salida.close();

			system("cat network");

		}
		else
			apertura = false;
		
		free(misdatos);
	}
	
	
	return apertura;	
}


void free(tDatos &datos) {

  delete[] datos.lista;
  datos.capacidad = 0;
  datos.num_datos = 0;
// datos.lista = null;

}