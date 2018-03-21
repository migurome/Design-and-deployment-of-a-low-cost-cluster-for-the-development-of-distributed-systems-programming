package miau;

import java.io.*;
import java.net.*;

class TCPCliente {

	public static void main(String args[]) {
		System.out.println("El cliente se pone en marcha, un momento mientras se leen los parametros");
		int port = 2510;
		String frase = null;
		String fraseModificada = null;
		String direccion = null;

		if (args.length == 0 || args == null) {
			System.out.println("La direccion IP es obligatoria, el programa terminara");
			System.exit(0);
		} else if (args.length < 4) {
			// ERROR SIEMPRE: EL NUMERO DE ARGUMENTOS OBLIGATORIOS ES 4
			System.out.println("El numero de argumentos es erroneo, minimo direccion y palabra: d direccion m palabra");
			System.exit(0);

		}

		else if (args.length > 4) {

			if (args[4].equals("p")) {
				if (args.length < 6) {

					System.out.println("No se ha especificado puerto con opcion p; el programa terminara");
					System.exit(0);
				} else {
					port = Integer.parseInt(args[5]);
				}

				System.out.println("Vamos a usar el puerto:" + port);

			}
		}

		if (args[0].equals("d")) {
			direccion = (args[1]);
			System.out.println("Vamos a usar la direccion IP:" + direccion);
		} else { // si no ponemos d, que es obligatorio, fallara
			System.out.println("El parametro debe ser d, el programa terminara");
			System.exit(0);
		}

		if (args[2].equals("m")) {
			frase = args[3];
		} else {// si no ponemos m, que es obligatorio, fallara
			System.out.println("El parametro debe ser m, el programa terminara");
			System.exit(0);
		}

		Socket socketCliente = null;
		try {
			socketCliente = new Socket(direccion, port);
		} catch (UnknownHostException e) {
			System.out.println("El host no existe");
			System.exit(0);
		} catch (IOException e) {
			System.out.println("***No se ha podido conectar: El host no esta levantado");
			System.exit(0);
		}
		DataOutputStream salidaServidor = null;
		try {
			salidaServidor = new DataOutputStream(socketCliente.getOutputStream());
		} catch (IOException e1) {
			System.out.println("No se ha podido obtener el DataOutputStream");
		}
		BufferedReader entradaDesdeServidor = null;
		try {
			entradaDesdeServidor = new BufferedReader(new InputStreamReader(socketCliente.getInputStream()));
		} catch (IOException e1) {
			System.out.println("No se ha podido obtener el BufferedReader");
		}
		try {
			salidaServidor.writeBytes(frase + 'n');
		} catch (IOException e) {
			System.out.println("No se ha podido escribir en cliente");
		}
		try {
			fraseModificada = entradaDesdeServidor.readLine();
		} catch (IOException e) {
			System.out.println("No se ha podido leer la linea");
		}
		System.out.println("DEL SERVIDOR: " + fraseModificada);

		InputStream aux = null;
		try {
			aux = socketCliente.getInputStream();
		} catch (IOException e) {
			System.out.println("No se ha podido obtener el InputStream");
		}
		DataInputStream flujo = new DataInputStream(aux);
		try {
			System.out.println(flujo.readUTF());
		} catch (IOException e) {
			System.out.println("Error al obtener datos del servidor");
		}

		InputStream aux2 = null;
		try {
			aux2 = socketCliente.getInputStream();
		} catch (IOException e) {
			System.out.println("No se ha podido obtener el InputStream");
		}
		DataInputStream flujo2 = new DataInputStream(aux2);
		try {
			System.out.println(flujo2.readUTF());
		} catch (IOException e) {
			System.out.println("Error al obtener datos del servidor");
		}

		try {
			socketCliente.close();
		} catch (IOException e) {
			System.out.println("Error al cerrar el socket");
		}
	}
}
