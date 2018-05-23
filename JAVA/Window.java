package ClientTFG;

import java.awt.BorderLayout;
import java.awt.FlowLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.BufferedReader;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.Socket;
import java.security.InvalidKeyException;
import java.security.Key;
import java.security.NoSuchAlgorithmException;

import javax.crypto.Cipher;
import javax.crypto.KeyGenerator;
import javax.crypto.NoSuchPaddingException;
import javax.crypto.SecretKey;
import javax.crypto.spec.SecretKeySpec;
import javax.swing.JButton;
import javax.swing.JDialog;
import javax.swing.JFileChooser;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JTextField;

public class Window extends JFrame {

	private JLabel serverIpLabel;
	private JTextField serverIpTextField;
	private JTextField filePathTextField;
	private JButton selectButton;
	private JButton sendButton;
	private JButton stopButton;
	private final JFileChooser fc = new JFileChooser();
	private File selectFile;
	private Cipher cipher;
	
	public Window() {
		try {
			cipher=Cipher.getInstance("AES");
			String myKey = "qwertyuiopasdfgh";
			SecretKey key = new SecretKeySpec(myKey.getBytes(), "AES");
			cipher.init(Cipher.ENCRYPT_MODE, key);
			
		} catch (NoSuchAlgorithmException | NoSuchPaddingException | InvalidKeyException e1) {
			// TODO Auto-generated catch block
			e1.printStackTrace();
		}
		this.selectFile = null;
		serverIpLabel = new JLabel("Server Address: ");
		serverIpTextField = new JTextField(40);
		serverIpTextField.setText("localhost");
		filePathTextField = new JTextField(40);
		selectButton = new JButton("Select");
		sendButton = new JButton("Send");
		stopButton = new JButton("Stop Server");
		this.setLayout(new BorderLayout());
		JPanel panelNorth = new JPanel(new FlowLayout());
		panelNorth.add(serverIpLabel);
		panelNorth.add(serverIpTextField);
		JPanel panelCenter = new JPanel(new FlowLayout());
		JPanel panelSouth = new JPanel(new FlowLayout());
		panelCenter.add(filePathTextField);
		panelCenter.add(selectButton);
		panelSouth.add(sendButton);
		panelSouth.add(stopButton);
		this.add(panelNorth, BorderLayout.NORTH);
		this.add(panelCenter, BorderLayout.CENTER);
		this.add(panelSouth, BorderLayout.SOUTH);
		this.setTitle("SEND FILES");
		
		stopButton.addActionListener(new ActionListener() {
			
			@Override
			public void actionPerformed(ActionEvent e) {
				stopAction();
				
			}
		});
		
		selectButton.addActionListener(new ActionListener() {
			
			@Override
			public void actionPerformed(ActionEvent e) {
				int returnVal = fc.showOpenDialog(Window.this);
				if(returnVal == JFileChooser.APPROVE_OPTION) {
					selectFile = fc.getSelectedFile();
					filePathTextField.setText(selectFile.getAbsolutePath());
				}
				else {
					//TODO cifrar el archivo cuando lo vaya a mandar para que el servidor lo 
				}
				
			}
		});
		
		sendButton.addActionListener(new ActionListener() {
			
			@Override
			public void actionPerformed(ActionEvent e) {
				sendAction();				
			}
		});
		
		//Agrupa la interfaz en el minimo tamaño posible y ademas le da fresquito
		this.pack();
	}
	
	
	private void sendAction() {
		String serverAddress = serverIpTextField.getText();
		if(selectFile == null) {
			JOptionPane.showMessageDialog(this, "You must select a file","Warning", JOptionPane.WARNING_MESSAGE);

		}
		else if((serverAddress == null)||(serverAddress.isEmpty())){
			JOptionPane.showMessageDialog(this, "You must define a server address","Warning", JOptionPane.WARNING_MESSAGE);

		}
		else {
			try {
				Socket s=new Socket(serverAddress,5000);
				DataInputStream din = new DataInputStream(s.getInputStream());  
				DataOutputStream dout = new DataOutputStream(s.getOutputStream());  
				FileReader br = new FileReader(selectFile);
				String line;
				long size = selectFile.length();
				String fileName = selectFile.getName();
				dout.writeUTF(fileName);
				dout.writeLong(size);
				int count;
				byte[] buffer = new byte[8192];
				byte[] bufferCiph = new byte[8192];
				char[] buffc = new char[8192];
				while((count = br.read(buffc)) > 0) {
					for(int i=0; i < count; ++i) {
						buffer[i] = (byte) buffc[i];	
					}
					bufferCiph = cipher.update(buffer, 0, buffer.length);
					dout.write(bufferCiph);
				}
				JOptionPane.showMessageDialog(this, "File successfully sent","File sent", JOptionPane.INFORMATION_MESSAGE);
				dout.close();
				din.close();
				br.close();
				s.close();

			} catch (IOException e) {
				JOptionPane.showMessageDialog(this, e.getMessage(),"Error", JOptionPane.ERROR_MESSAGE);
			}

			
		}
			
		
	}
	
	private void stopAction() {
		
		String serverAddress = serverIpTextField.getText();
		if((serverAddress == null)||(serverAddress.isEmpty())){
			JOptionPane.showMessageDialog(this, "You must define a server address","Warning", JOptionPane.WARNING_MESSAGE);

		}
		else {
			try {
				Socket s=new Socket(serverAddress,5000);
				DataInputStream din = new DataInputStream(s.getInputStream());  
				DataOutputStream dout = new DataOutputStream(s.getOutputStream());  
				String line;
				dout.writeUTF("Stop");
				JOptionPane.showMessageDialog(this, "Server stopped","Server stopped", JOptionPane.INFORMATION_MESSAGE);
				dout.close();
				din.close();
				s.close();

			} catch (IOException e) {
				JOptionPane.showMessageDialog(this, e.getMessage(),"Error", JOptionPane.ERROR_MESSAGE);
			}

			
		}
			
	}
	
	
}
 