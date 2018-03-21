package Sc;

/*File Server****************************************************/

import java.io.*;
import java.net.*;

public class fileserver {
	public static void main(String[] args) throws Exception {
		ServerSocket s=new ServerSocket(5643);
		Socket sr=s.accept();
		FileInputStream fr=new FileInputStream("C:/hlocal/miau.txt");
		/* include your file name which you want to move*/
		byte b[]=new byte[2002];
		fr.read(b, 0, b.length);
		OutputStream os=sr.getOutputStream();
		os.write(b, 0, b.length);

	}
}