/***
 *                     _        _    ____ _ _            _   
 *      ___  ___   ___| | _____| |_ / ___| (_) ___ _ __ | |_ 
 *     / __|/ _ \ / __| |/ / _ \ __| |   | | |/ _ \ '_ \| __|
 *     \__ \ (_) | (__|   <  __/ |_| |___| | |  __/ | | | |_ 
 *     |___/\___/ \___|_|\_\___|\__|\____|_|_|\___|_| |_|\__|
 *                                                           
 */

package Client;

import java.io.BufferedReader;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.EOFException;
import java.io.File;
import java.io.FileOutputStream;
import java.io.InputStreamReader;
import java.net.Socket;
import java.util.Scanner;

class Client{
	
	public static void main(String args[])throws Exception{  
	
		String address = "";
		Scanner sc=new Scanner(System.in);
		System.out.println("Enter Server Address: ");
		address=sc.nextLine();
		
		//create the socket on port 5000
		Socket s=new Socket(address,5000);
		
		DataInputStream din = new DataInputStream(s.getInputStream());  
		DataOutputStream dout = new DataOutputStream(s.getOutputStream());  
		BufferedReader br = new BufferedReader(new InputStreamReader(System.in));  

		System.out.println("Send Get to start...");
		String str = "", filename = "";

		try{

			dout.writeUTF(str); 
			dout.flush();  

			filename=din.readUTF(); 
			System.out.println("Receving file: "+filename);
			System.out.println("Saving as file: "+filename);
			//
			long sz=Long.parseLong(din.readUTF());
			System.out.println ("File Size: "+(sz/(1024*1024))+" MB");

			byte b[]=new byte [1024];
			System.out.println("Receving file..");
			FileOutputStream fos=new FileOutputStream(new File(filename),true);
			long bytesRead;
			
			do
			{
				bytesRead = din.read(b, 0, b.length);
				fos.write(b,0,b.length);
			}while(!(bytesRead<1024));

			System.out.println("Completed");
			fos.close(); 
			dout.close();  	
			s.close();  
		}
		catch(EOFException e){}
	}
}  
//this is a typical client program 