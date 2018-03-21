import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.DataInput;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;

import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JTextArea;
import javax.swing.JTextField;

public class Servidor extends JFrame implements Runnable {
	JTextArea mensaje;
	JButton boton;

	public Servidor() {

		mensaje = new JTextArea();
		mensaje.setBounds(10,10,400,400);
		add(mensaje);
		setLayout(null);
		setSize(500,500);
		setVisible(true);
		Thread thread = new Thread(this);
		thread.start();

	}
	public static void main(String[] args) {
		new Servidor();
	}

	@Override
	public void run() {
		try {
			ServerSocket ss = new ServerSocket(8080);
			Socket socket = ss.accept();
			while(true) {
				DataInputStream in = new DataInputStream(socket.getInputStream());
				String msg = in.readUTF();
				mensaje.append("\n"+socket.getInetAddress()+ " : " + msg);
				socket.close();
				if(msg.equalsIgnoreCase("FIN")){
					ss.close();
					break;	
				}
			}
		}
		catch(Exception e) {
			e.printStackTrace();
		}
	}



}

