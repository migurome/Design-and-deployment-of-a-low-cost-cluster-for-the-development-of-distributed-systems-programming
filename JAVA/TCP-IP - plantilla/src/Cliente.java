import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;

import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JTextField;

public class Cliente extends JFrame implements ActionListener {
	JTextField mensaje;
	JButton boton;

	public Cliente() {

		mensaje = new JTextField();
		mensaje.setBounds(10,10,200,20);
		add(mensaje);

		boton = new JButton();
		boton.setBounds(10,40,150,20);
		boton.setText("Enviar");
		boton.addActionListener(this);
		add(boton);

		setLayout(null);
		setSize(400,400);
		setVisible(true);
	}
	public static void main(String[] args) {
		new Cliente();
	}
	@Override
	public void actionPerformed(ActionEvent a) {

		if(a.getSource() == boton) {
			try {
				Socket socket = new Socket("127.0.0.1",8080);
				//El fujo de salida será el del socket
				DataOutputStream out = new DataOutputStream(socket.getOutputStream());
				out.writeUTF(mensaje.getText());
				socket.close();
			}
			catch(Exception e) {
				System.out.println("Error");
			}
		}
	}



}

