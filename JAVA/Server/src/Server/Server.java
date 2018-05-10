/***
 *                     _        _   ____                           
 *      ___  ___   ___| | _____| |_/ ___|  ___ _ ____   _____ _ __ 
 *     / __|/ _ \ / __| |/ / _ \ __\___ \ / _ \ '__\ \ / / _ \ '__|
 *     \__ \ (_) | (__|   <  __/ |_ ___) |  __/ |   \ V /  __/ |   
 *     |___/\___/ \___|_|\_\___|\__|____/ \___|_|    \_/ \___|_|   
 *                                                                 
 */

package Server;

import java.io.BufferedWriter;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.FileWriter;
import java.net.ServerSocket;
import java.net.Socket;
import java.nio.file.Files;
import java.util.Scanner;

class Server{

	public static void main(String args[])throws Exception{  

		//Se declara un string y pide un nombre de archivo para leerlo
		String fileName;

		while(true){
			//Crea un server socket en un puerto fijo, en este caso el 5000
			ServerSocket ss=new ServerSocket(5000); 
			System.out.println ("Waiting for request");
			Socket s=ss.accept();  
			System.out.println ("Connected With "+s.getInetAddress().toString());
			DataInputStream din=new DataInputStream(s.getInputStream());  
			DataOutputStream dout=new DataOutputStream(s.getOutputStream());
			FileOutputStream bw = null;
			try{

				fileName = din.readUTF();
				System.out.println("Receiving File: "+fileName);
				//Lo manda a la carpeta por defecto files
				File file = new File(fileName);
				bw = new FileOutputStream(file);
				//Aqui ya podemos escribir

				long size = din.readLong();

				String line;
				int count;
				byte[] buffer = new byte[8192];
				char[] buffc = new char[8192];
				
				while((count = din.read(buffer)) > 0){
					bw.write(buffer);
				}
				bw.close();
				
			}


			catch(Exception e)
			{
			
			}
			//Siempre se jeecuta da igual la rama por la que vayas
			finally{
				bw.close();
				din.close();
				dout.close();
				ss.close();

			}
		}
	}
}  