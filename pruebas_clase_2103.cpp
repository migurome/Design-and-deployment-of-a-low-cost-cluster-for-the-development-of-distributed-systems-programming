#include <iostream>
#include <string>
#include <fstream>

using namespace std;

const int NUM_ALUMNOS = 10;

typedef enum tGenero { M, H };

typedef struct {

	string nombre;
	int edad;
	tGenero genero;

} tAlumno;

typedef tAlumno tClase[NUM_ALUMNOS];


/***
*       __                  _
*      / _|_   _ _ __   ___(_) ___  _ __   ___  ___
*     | |_| | | | '_ \ / __| |/ _ \| '_ \ / _ \/ __|
*     |  _| |_| | | | | (__| | (_) | | | |  __/\__ \
*     |_|  \__,_|_| |_|\___|_|\___/|_| |_|\___||___/
*
*/

int cargar(string nombre, tClase miclase);
void mostrar(const tClase miclase, int num_elementos);
float media(const tClase miclase, int num_elementos);
int mayor(const tClase miclase, int num_elementos);
tGenero string2genero(const string migenero);
string gener2string(tGenero migenero);
int numMayoresEdad(const tClase miclase, int num_elementos, int edad);
int posicionAlumno(const tClase miclase, int num_elementos, const string nombre);
bool estaEnClase(const tClase miclase, int num_elementos, const string nombre);
int numMujeres(const tClase miclase, int num_elementos);
bool guardar(const tClase miclase, int num_elementos, string nombre);
void pedirFicheroUsuario(string nombre_fichero);

bool cumpleanios(tClase miclase, int num_elementos, string nombre);
/***
 *                      _       
 *      _ __ ___   __ _(_)_ __  
 *     | '_ ` _ \ / _` | | '_ \ 
 *     | | | | | | (_| | | | | |
 *     |_| |_| |_|\__,_|_|_| |_|
 *                              
 */

void main() {


	string nombre_fichero = "alumnos.txt";
	tClase prueba_alumnos;
	int pos_alumno;

	int num_alumnos_cargados = cargar(nombre_fichero, prueba_alumnos);

	int posicion = posicionAlumno(prueba_alumnos, num_alumnos_cargados, "Miguel");

	cout << "La edad de miguel es: " << prueba_alumnos[posicion].edad << endl;
	cumpleanios(prueba_alumnos, num_alumnos_cargados, "Miguel");
	cout << "La edad de miguel es: " << prueba_alumnos[posicion].edad << endl;

	system("Pause");

}


int cargar(string nombre, tClase miclase) {


	ifstream entrada;
	string  cadena_genero;
	int i = 0;

	entrada.open(nombre);


	if (entrada.is_open() == true) {

		while ((entrada.eof() == false) && (i < NUM_ALUMNOS)) {

			entrada >> miclase[i].nombre;
			entrada >> miclase[i].edad;

			entrada >> cadena_genero;

			miclase[i].genero = string2genero(cadena_genero);
			i++;
		}

		entrada.close();
	}


	return i;

}

void mostrar(const tClase miclase, int num_elementos) {

	for (int i = 0; i < num_elementos; i++) {
		cout << miclase[i].nombre << " " << miclase[i].edad << endl;
	}
}

float media(const tClase miclase, int num_elementos) {

	float media = 0;
	int suma_acumulada = 0;

	for (int i = 0; i < num_elementos; i++) {
		suma_acumulada = suma_acumulada + miclase[i].edad;
	}

	if (num_elementos > 0)
		media = suma_acumulada / num_elementos;

	return media;
}

int mayor(const tClase miclase, int num_elementos) {

	int mayor = 0;

	for (int i = 0; i < num_elementos; i++)
		if (miclase[i].edad > mayor)
			mayor = miclase[i].edad;

	return mayor;
}

tGenero string2genero(const string migenero) {

	tGenero retorno = H;

	if (migenero == "M")
		retorno = M;

	return retorno;
}

string gener2string(tGenero migenero) {

	string cadena = "Hombre";

	if (migenero == M)
		cadena = "Mujer";

	return cadena;
}

int numMayoresEdad(const tClase miclase, int num_elementos, int edad) {

	int contador = 0;

	for (int i = 0; i < num_elementos; i++)
		if (miclase[i].edad >= edad)
			contador++;

	return contador;
}

int posicionAlumno(const tClase miclase, int num_elementos, const string nombre) {

	int posicion = -1;
	int i = 0;

	while (miclase[i].nombre != nombre && i < num_elementos)
		i++;

	if (miclase[i].nombre == nombre)
		posicion = i;

	return posicion;
}

bool estaEnClase(const tClase miclase, int num_elementos, const string nombre) {

	int i = 0;
	
	while (miclase[i].nombre != nombre && i < num_elementos)
		i++;

	return miclase[i].nombre == nombre;
}


int numMujeres(const tClase miclase, int num_elementos) {

	int contador = 0;


	for (int i = 0; i < num_elementos; i++)
		if (miclase[i].genero == M)
			contador++;

	return contador;
}

bool guardar(const tClase miclase, int num_elementos, string nombre) {

	bool abierto = false;
	ofstream salida;


	salida.open(nombre);

	if (salida.is_open()) {
		abierto = true;

		for (int i = 0; i < num_elementos; i++) 
			salida << miclase[i].nombre << " " << miclase[i].edad << " " << miclase[i].genero << endl;

		salida.close();
	}
	
	return abierto;
}

void pedirFicheroUsuario(string nombre_fichero) {

	cout << "Introduce un nombre de fichero: " << endl;
	cin >> nombre_fichero;

}

//Aumenta la edad e una persona de clase
bool cumpleanios(tClase miclase, int num_elementos, string nombre) {

	bool esta_y_es_sucumple = false;
	int posicion;

	if (estaEnClase(miclase, num_elementos, nombre) == true) {
		esta_y_es_sucumple = true;
		posicion = posicionAlumno(miclase, num_elementos, nombre);
		miclase[posicion].edad++;
	}

	return esta_y_es_sucumple;

}