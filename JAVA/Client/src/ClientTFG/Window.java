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
	private final JFileChooser fc = new JFileChooser();
	private File selectFile;
	
	public Window() {
		this.selectFile = null;
		serverIpLabel = new JLabel("Server Address: ");
		serverIpTextField = new JTextField(40);
		serverIpTextField.setText("localhost");
		filePathTextField = new JTextField(40);
		selectButton = new JButton("Select");
		sendButton = new JButton("Send");
		this.setLayout(new BorderLayout());
		JPanel panelNorth = new JPanel(new FlowLayout());
		panelNorth.add(serverIpLabel);
		panelNorth.add(serverIpTextField);
		JPanel panelCenter = new JPanel(new FlowLayout());
		panelCenter.add(filePathTextField);
		panelCenter.add(selectButton);
		this.add(panelNorth, BorderLayout.NORTH);
		this.add(panelCenter, BorderLayout.CENTER);
		this.add(sendButton, BorderLayout.SOUTH);
		this.setTitle("SEND FILES");
		
		selectButton.addActionListener(new ActionListener() {
			
			@Override
			public void actionPerformed(ActionEvent e) {
				int returnVal = fc.showOpenDialog(Window.this);
				if(returnVal == JFileChooser.APPROVE_OPTION) {
					selectFile = fc.getSelectedFile();
					filePathTextField.setText(selectFile.getAbsolutePath());
				}
				else {
					//No hacer nada
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
				char[] buffc = new char[8192];
				while((count = br.read(buffc)) > 0) {
					for(int i=0; i < count; ++i) {
						buffer[i] = (byte) buffc[i];	
					}
	
					dout.write(buffer);
				}
				JOptionPane.showMessageDialog(this, "File successfully sent","File sent", JOptionPane.INFORMATION_MESSAGE);
				dout.close();
				din.close();
				br.close();
				s.close();

			} catch (IOException e) {
				JOptionPane.showMessageDialog(this, e.getMessage(),"Error", JOptionPane.ERROR);
			}

			
		}
			
		
	}
	
	
}
 