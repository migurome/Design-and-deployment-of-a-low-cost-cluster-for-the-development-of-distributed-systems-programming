package ClientTFG;

import java.awt.BorderLayout;
import java.awt.FlowLayout;
import java.awt.GridLayout;
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
import javax.swing.BorderFactory;
import javax.swing.JButton;
import javax.swing.JDialog;
import javax.swing.JFileChooser;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JProgressBar;
import javax.swing.JTextField;
import javax.swing.SwingUtilities;
import javax.swing.event.DocumentEvent;
import javax.swing.event.DocumentListener;

public class Window extends JFrame {

	private JTextField serverIpTextField;
	private JTextField filePathTextField;
	private JButton selectButton;
	private JButton sendButton;
	private JButton stopButton;
	private JButton connectButton;
	private final JFileChooser fc = new JFileChooser();
	private File selectFile;
	private Cipher cipher;
	private Socket socket;
	private DataInputStream din;
	private DataOutputStream dout;
	private JLabel statusLabel;
	private JProgressBar progressBar;
	private double progress;


	public Window() {

		this.setLayout(new BorderLayout());
		this.setTitle("File Sending Tool");

		initCrypto();
		initGui();
		updateButtons();
		//Agrupa la interfaz en el minimo tamaño posible y además refresca
		this.pack();
		this.setDefaultCloseOperation(EXIT_ON_CLOSE);
	}

	private void initCrypto() {
		try {
			cipher=Cipher.getInstance("AES");
			String myKey = "ProgramacionDeSistemasDistribuidos";
			SecretKey key = new SecretKeySpec(myKey.getBytes(), "AES");
			cipher.init(Cipher.ENCRYPT_MODE, key);

		} catch (NoSuchAlgorithmException | NoSuchPaddingException | InvalidKeyException e1) {
			e1.printStackTrace();
		}
	}

	private void initNorth() {
		serverIpTextField = new JTextField(35);
		serverIpTextField.setText("localhost");
		connectButton = new JButton("Connect");
		JPanel panelNorth = new JPanel(new FlowLayout());
		panelNorth.add(serverIpTextField);
		panelNorth.add(connectButton);
		serverIpTextField.getDocument().addDocumentListener(new DocumentListener() {

			@Override
			public void removeUpdate(DocumentEvent e) {
				updateButtons();

			}

			@Override
			public void insertUpdate(DocumentEvent e) {
				updateButtons();

			}

			@Override
			public void changedUpdate(DocumentEvent e) {
				updateButtons();

			}
		});;
		connectButton.addActionListener(new ActionListener() {

			@Override
			public void actionPerformed(ActionEvent e) {
				if (socket==null) {
					connectAction();
				}
				else {
					disconnectAction();
				}

				updateButtons();
			}
		});
		panelNorth.setBorder(BorderFactory.createTitledBorder("Server Address"));
		this.add(panelNorth, BorderLayout.NORTH);

	}



	private void initCenter() {
		this.selectFile = null;
		filePathTextField = new JTextField(40);
		selectButton = new JButton("Select");
		selectButton.addActionListener(new ActionListener() {

			@Override
			public void actionPerformed(ActionEvent e) {
				int returnVal = fc.showOpenDialog(Window.this);
				if(returnVal == JFileChooser.APPROVE_OPTION) {
					selectFile = fc.getSelectedFile();
					filePathTextField.setText(selectFile.getAbsolutePath());
				}
				updateButtons();
			}
		});

		JPanel panelCenter = new JPanel(new FlowLayout());
		panelCenter.add(filePathTextField);
		panelCenter.add(selectButton);
		panelCenter.setBorder(BorderFactory.createTitledBorder("Files"));

		this.add(panelCenter, BorderLayout.CENTER);


	}

	private void initSouth() {
		sendButton = new JButton("Send");
		stopButton = new JButton("Stop Server");
		statusLabel = new JLabel("Idle ");
		progressBar = new JProgressBar(0,100);
		progressBar.setValue(0);
		progressBar.setStringPainted(true);
		JPanel panelSouth = new JPanel(new GridLayout(1,2));
		JPanel panelStatus = new JPanel();
		JPanel panelActions = new JPanel();
		panelStatus.setBorder(BorderFactory.createTitledBorder("Status"));
		panelActions.setBorder(BorderFactory.createTitledBorder("Actions"));
		panelStatus.add(statusLabel);
		panelStatus.add(progressBar);
		panelActions.add(sendButton);
		//Este botón es para testear la desconexión del servidor
		//panelActions.add(stopButton);
		panelSouth.add(panelStatus);
		panelSouth.add(panelActions);
		stopButton.addActionListener(new ActionListener() {

			@Override
			public void actionPerformed(ActionEvent e) {
				stopAction();
				updateButtons();

			}
		});


		sendButton.addActionListener(new ActionListener() {

			
			@Override
			public void actionPerformed(ActionEvent e) {
			
				enableButtons(false);
				Thread thread = new Thread() {
					public void run() {
						sendAction();
					}
				};
				thread.start();
				
			}
		});

		this.add(panelSouth, BorderLayout.SOUTH);

	}

	private void initGui() {
		initNorth();
		initCenter();
		initSouth();
	}

	private void sendAction() {
		if(selectFile == null) {
			JOptionPane.showMessageDialog(this, "You must select a file","Warning", JOptionPane.WARNING_MESSAGE);

		}
		else {
			try {
				FileReader br = new FileReader(selectFile);
				String line;
				long size = selectFile.length();
				String fileName = selectFile.getName();
				dout.writeUTF(fileName);
				dout.writeLong(size);
				int count;
				progress = 0;
				
				byte[] buffer = new byte[8192];
				byte[] bufferCiph = new byte[8192];
				char[] buffc = new char[8192];
				while((count = br.read(buffc)) > 0) {
					for(int i=0; i < count; ++i) {
						buffer[i] = (byte) buffc[i];	
					}
					bufferCiph = cipher.update(buffer, 0, buffer.length);
					dout.write(bufferCiph);
					progress+=(count*1.0)/size;
					SwingUtilities.invokeLater(new Runnable() {
						
						@Override
						public void run() {
							progressBar.setValue((int) (progress*100));
							statusLabel.setText("Sending ");
							
						}
					});
				
				}
				SwingUtilities.invokeLater(new Runnable() {
					
					@Override
					public void run() {
						updateButtons();
						statusLabel.setText("File sent ");
					}
				});
				JOptionPane.showMessageDialog(this, "File successfully sent","File sent", JOptionPane.INFORMATION_MESSAGE);
				br.close();

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

	private void connectAction() {
		String serverAddress = serverIpTextField.getText();
		if((serverAddress == null)||(serverAddress.isEmpty())){
			JOptionPane.showMessageDialog(this, "You must define a server address","Warning", JOptionPane.WARNING_MESSAGE);

		}
		else {
			try {
				socket = new Socket(serverAddress,5000);
				din  = new DataInputStream(socket.getInputStream()); 
				dout =  new DataOutputStream(socket.getOutputStream());  

			} catch (IOException e) {
				e.printStackTrace();
			}
		}
	}

	private void disconnectAction() {

		if(socket!=null) {
			try {
				din.close();
				dout.close();
				socket.close();
			} catch (IOException e) {
			}
			din = null;
			dout = null;
			socket = null;
		}
	}

	private void updateButtons() {
		if(socket!=null) {
			connectButton.setEnabled(true);
			connectButton.setText("Disconnect");
			stopButton.setEnabled(true);
			if(selectFile != null) {
				sendButton.setEnabled(true);
			}
			else {
				sendButton.setEnabled(false);
			}
		}
		else {
			connectButton.setText("Connect");
			sendButton.setEnabled(false);
			stopButton.setEnabled(false);
			if(serverIpTextField.getText().isEmpty()) {
				connectButton.setEnabled(false);
			}
			else {
				connectButton.setEnabled(true);
			}
		}

	}
	
	private void enableButtons(Boolean enabled) {
		connectButton.setEnabled(enabled);
		selectButton.setEnabled(enabled);
		stopButton.setEnabled(enabled);
		sendButton.setEnabled(enabled);
	}


}
