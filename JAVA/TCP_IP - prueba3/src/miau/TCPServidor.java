package miau;

import java.io.*;
import java.net.*;
class TCPServidor
{

	public static void main(String args[]) throws Exception
	{
		System.out.println("El servidor va a ponerse a la escucha, un momento mientras se leen los parametros");
		int port = 2510;
		if (args.length == 0 || args==null) {
			System.out.println("El puerto no se ha especificado, se usara el puerto por defecto: 2510");
		} else if (args[0].equals("p")) {
			port = Integer.parseInt(args[1]);
			System.out.println("Vamos a usar el puerto:"+port);
		}
		else    {
			System.out.println("Debes especificar la opcion p");
		}
		String fraseCliente;
		String fraseMayusculas;
		ServerSocket socketAtendido = null;
		try {
			socketAtendido = new ServerSocket(port);
		} catch (IOException e)
		{
			System.out.println("No se ha podido levantar el servidor");
			System.exit ( 0 );
		}
		System.out.println("Servidor a la escucha");
		while (true) {
			Socket socketConexion = null;
			try {
				socketConexion = socketAtendido.accept();
			} catch (IOException e)
			{
				System.out.println("No se ha podido crear el nuevo socket");
			}
			//devuelve lo introducido en el cliente pero en mayusculas
			BufferedReader entradaDesdeCliente = null;
			try {
				entradaDesdeCliente = new BufferedReader(new InputStreamReader(socketConexion.getInputStream()));
			} catch (IOException e)
			{
				System.out.println("Error en el flujo de datos de entrada");
			}

			DataOutputStream salidaACliente = null;
			try {
				salidaACliente = new DataOutputStream(socketConexion.getOutputStream());
			} catch (IOException e)
			{
				System.out.println("Error en el flujo de datos de salida");
			}

			fraseCliente = entradaDesdeCliente.readLine();
			fraseMayusculas = fraseCliente.toUpperCase() + 'n';
			salidaACliente.writeBytes(fraseMayusculas);
			//devuelve la direccion IP del cliente
			OutputStream aux = null;
			try {
				aux = socketConexion.getOutputStream();
			} catch (IOException e)
			{
				System.out.println("Error obtener socket");
			}
			DataOutputStream flujo = new DataOutputStream(aux);
			try {
				flujo.writeUTF ( "Tu direccion IP es:"+socketConexion.getInetAddress() );
			} catch (IOException e)
			{
				System.out.println("Error al escribir al cliente");
			}
			//devuelve el puerto usado por el cliente
			OutputStream aux2 = null;
			try {
				aux2 = socketConexion.getOutputStream();
			} catch (IOException e)
			{
				System.out.println("Error obtener socket");
			}
			DataOutputStream flujo2 = new DataOutputStream(aux2);
			try {
				flujo2.writeUTF ( "Tu puerto es:"+socketConexion.getLocalPort() );
			} catch (IOException e)
			{
				System.out.println("Error al escribir al cliente");
			}
		}
	}
}
