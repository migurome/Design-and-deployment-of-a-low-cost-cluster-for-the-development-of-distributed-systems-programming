/***
 *                     _        _   ____                           
 *      ___  ___   ___| | _____| |_/ ___|  ___ _ ____   _____ _ __ 
 *     / __|/ _ \ / __| |/ / _ \ __\___ \ / _ \ '__\ \ / / _ \ '__|
 *     \__ \ (_) | (__|   <  __/ |_ ___) |  __/ |   \ V /  __/ |   
 *     |___/\___/ \___|_|\_\___|\__|____/ \___|_|    \_/ \___|_|   
 *                                                                 
 */

package Server;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.Scanner;

class Server{
	
	public static void main(String args[])throws Exception{  
		
		//Se declara un string y pide un nombre de archivo para leerlo
		String filename;
		System.out.println("Enter File Name: ");
		Scanner sc=new Scanner(System.in);
		
		filename=sc.nextLine();
		
		while(true)
		{
			//Crea un server socket en un puerto fijo, en este caso el 5000
			ServerSocket ss=new ServerSocket(5000); 
			System.out.println ("Waiting for request");
			Socket s=ss.accept();  
			System.out.println ("Connected With "+s.getInetAddress().toString());
			DataInputStream din=new DataInputStream(s.getInputStream());  
			DataOutputStream dout=new DataOutputStream(s.getOutputStream());
			
			try{
				String str="";  

				str=din.readUTF();
				System.out.println("SendGet....Ok");

				if(!str.equals("stop")){  

					System.out.println("Sending File: "+filename);
					dout.writeUTF(filename);  
					dout.flush();  

					File f = new File(filename);
					FileInputStream fin = new FileInputStream(f);
					long sz = (int) f.length();

					//Cambiado de 1024 a lo que hay
					byte b[] = new byte [1024];

					int read;

					dout.writeUTF(Long.toString(sz)); 
					dout.flush(); 

					System.out.println ("Size: " + sz);
					System.out.println ("Buf size: "+ss.getReceiveBufferSize());

					while((read = fin.read(b)) != -1){
						dout.write(b, 0, read); 
						dout.flush(); 
					}
					
					fin.close();

					System.out.println("..ok"); 
					dout.flush(); 
				}
				
				dout.writeUTF("stop");  
				System.out.println("Send Complete");
				dout.flush();  
			}
			catch(Exception e)
			{
				e.printStackTrace();
				System.out.println("An error occured");
			}
			din.close();  
			s.close();  
			ss.close();  
		}
	}
}  