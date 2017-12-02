import javax.swing.*;
import javax.swing.border.TitledBorder;

import java.awt.*;
import java.awt.event.*;
import java.text.NumberFormat;
import java.util.Arrays;

public class Yek extends JFrame {
	public static void main(String args[]){
		Yek gui = new Yek();
		gui.pack();
		gui.setVisible(true);
	}
	
	private YekSerial logic;
	
	//port selection stuff
	private JPanel selectPanel;
	private String portName;
	private JComboBox<String> ports;
	
	//fingerprint addition stuff
	private JPanel addFingerPanel;
	private JLabel addLabel;
	private JFormattedTextField fingerId;
	
	//fingerprint deletion stuff
	private JPanel delFingerPanel;
	private JLabel delLabel;
	private JFormattedTextField delFingerId;
	
	//stuff for adding accounts
	private JPanel addAccPanel;
	private JTextField addAccName;
	private JTextField addAccUser;
	private JPasswordField addAccPw;
	private JLabel addAccLabel;
	
	//stuff for deleting accounts
	private JPanel delAccPanel;
	private JTextField delAccName;
	private JLabel delAccLabel;
	
	public Yek () {
		super("Yek");
		//setSize(1000,700);
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		logic = new YekSerial();
		portName = "";
		Container content = this.getContentPane();
		content.setLayout(new BoxLayout(content, BoxLayout.PAGE_AXIS));
		createSelectPanel();
		createAddFingerprint();
		createDelFingerprint();
		createAddAcc();
		createDelAcc();
	}
	
	private void createDelAcc() {
		delAccName = new JTextField();
		JButton delAccButton = new JButton("Delete Account");
		delAccLabel = new JLabel("");
		ActionListener delAccListener = new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				if (portName == "") {
					//dont even do it
					delAccLabel.setText("Error! No Serial Port selected!");
					return;
				}
				String name = null;
				try {
					name = delAccName.getText();
				} catch (Exception ex) {
					return;
				}
				if (name.length() == 0) {
					delAccLabel.setText("Please Enter a valid account name");
					return;
				}
				int result = logic.del_acc(name + "\n");
				if (result == 0) {
					delAccLabel.setText(String.format("Account %s deleted", name));
				}
				else if (result == 1) {
					delAccLabel.setText(String.format("Could not locate account %s", name));
				}
			}
		};
		delAccButton.addActionListener(delAccListener);
		delAccPanel = new JPanel(new GridBagLayout());
		TitledBorder border = new TitledBorder("Delete An Account");
		border.setTitleJustification(TitledBorder.LEFT);
		border.setTitlePosition(TitledBorder.TOP);
		delAccPanel.setBorder(border);
		GridBagConstraints c = new GridBagConstraints();
		c.fill = GridBagConstraints.HORIZONTAL;
		c.ipadx = 200;
		c.ipady = 10;
		c.insets = new Insets(5,5,5,5);
		c.gridx = 0;
		c.gridy = 0;
		c.anchor = GridBagConstraints.CENTER;
		c.weightx = 0.2;
		delAccPanel.add(delAccName, c);
		c.gridy = 1;
		delAccPanel.add(delAccButton, c);
		c.gridy = 2;
		delAccPanel.add(delAccLabel, c);
		delAccPanel.setAlignmentX(Component.CENTER_ALIGNMENT);
		this.add(delAccPanel);
	}
	
	private void createAddAcc() {
		addAccName = new JTextField();
		addAccUser = new JTextField();
		addAccPw = new JPasswordField();
		addAccLabel = new JLabel("");
		JButton addAccButton = new JButton("Add Account");
		ActionListener addAccListener = new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				if (portName == "") {
					//dont even do it
					addAccLabel.setText("Error! No Serial Port selected!");
					return;
				}
				String name = null;
				String user = null;
				String pw = null;
				try {
					name = addAccName.getText();
					user = addAccUser.getText();
					pw = new String(addAccPw.getPassword());
					addAccPw.setText("");
				} catch (Exception ex) {
					addAccPw.setText("");
					return;
				}
				if (name.length() == 0) {
					addAccLabel.setText("Please Enter a valid account name");
					return;
				}
				else if (user.length() == 0) {
					addAccLabel.setText("Please Enter a valid username");
					return;
				}
				if (pw.length() == 0) {
					addAccLabel.setText("Please Enter a valid password");
					return;
				}
				int result = logic.add_acc(name+"\n", user+"\n", pw+"\n");
				if (result == 0) {
					addAccLabel.setText(String.format("Account %s added", name));
				}
				else if (result == 1) {
					addAccLabel.setText(String.format("Error adding account: %s", name));
				}
			}
		};
		addAccButton.addActionListener(addAccListener);
		addAccPanel = new JPanel(new GridBagLayout());
		TitledBorder border = new TitledBorder("Add An Account");
		border.setTitleJustification(TitledBorder.LEFT);
		border.setTitlePosition(TitledBorder.TOP);
		addAccPanel.setBorder(border);
		GridBagConstraints c = new GridBagConstraints();
		c.fill = GridBagConstraints.HORIZONTAL;
		c.ipadx = 200;
		c.ipady = 10;
		//top left bottom right
		c.insets = new Insets(1,5,1,5);
		c.gridx = 0;
		c.gridy = 0;
		c.anchor = GridBagConstraints.CENTER;
		c.weightx = 0.2;
		addAccPanel.add(new JLabel("Account Name:"), c);
		c.gridy = 1;
		addAccPanel.add(addAccName, c);
		c.gridy = 2;
		addAccPanel.add(new JLabel("Username:"), c);
		c.gridy = 3;
		addAccPanel.add(addAccUser,c);
		c.gridy = 4;
		addAccPanel.add(new JLabel("Password:"), c);
		c.gridy = 5;
		addAccPanel.add(addAccPw, c);
		c.gridy = 6;
		c.insets = new Insets(10,5,5,5);
		addAccPanel.add(addAccButton, c);
		c.gridy = 7;
		addAccPanel.add(addAccLabel, c);
		addAccPanel.setAlignmentX(Component.CENTER_ALIGNMENT);
		this.add(addAccPanel);
	}
	
	private void createDelFingerprint() {
		//create the text field specifying the fingerprint #
		delFingerId = new JFormattedTextField(NumberFormat.getIntegerInstance());
		delLabel = new JLabel("Specify a Fingerprint ID number");
		JButton del = new JButton("Delete Fingerprint");
		ActionListener delActionListener = new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				if (portName == "") {
					//dont even do it
					delLabel.setText("Error! No Serial Port selected!");
					return;
				}
				int i = 0;
				try {
					i = Integer.parseInt(delFingerId.getText());
				} catch (Exception ex) {
					delLabel.setText("Error: Please enter an integer from 1-20");
					return;
				}
			    
				int result = logic.delete_user(i);
				if (result == 0) {
					delLabel.setText("Success!");
				}
				else if (result == 1) {
					delLabel.setText("Error: No fingerprint currently registered for that ID number");
				}
				else {
					delLabel.setText("ERROR!");
				}
				return;
			}			
		};
		del.addActionListener(delActionListener);
		delFingerPanel = new JPanel(new GridBagLayout());
		TitledBorder border = new TitledBorder("Delete a Fingerprint");
		border.setTitleJustification(TitledBorder.LEFT);
		border.setTitlePosition(TitledBorder.TOP);
		delFingerPanel.setBorder(border);
		GridBagConstraints c = new GridBagConstraints();
		c.fill = GridBagConstraints.HORIZONTAL;
		c.ipadx = 100;
		c.ipady = 10;
		c.insets = new Insets(5,5,5,40);
		c.gridx = 0;
		c.gridy = 0;
		c.anchor = GridBagConstraints.CENTER;
		c.weightx = 0.2;
		delFingerPanel.add(delFingerId, c);
		c.insets = new Insets(5,5,5,5);
		c.ipadx = 10;
		c.gridx = 1;
		delFingerPanel.add(del, c);
		c.gridx = 0;
		c.gridy = 1;
		c.gridwidth = 2;
		c.ipadx = 200;
		delFingerPanel.add(delLabel, c);
		delFingerPanel.setAlignmentX(Component.CENTER_ALIGNMENT);
		this.add(delFingerPanel);
	}
	
	private void createAddFingerprint() {
		//create the text field specifying the fingerprint #
		fingerId = new JFormattedTextField(NumberFormat.getIntegerInstance());
		addLabel = new JLabel("Specify a Fingerprint ID number");
		JButton add = new JButton("Add Fingerprint");
		ActionListener addActionListener = new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				if (portName == "") {
					//dont even do it
					addLabel.setText("Error! No Serial Port selected!");
					return;
				}
				int i = 0;
				try {
					i = Integer.parseInt(fingerId.getText());
				} catch (Exception ex) {
					addLabel.setText("Error: Please enter an integer from 1-20");
					return;
				}
			    
				int result = logic.register_user(i);
				if (result == 0) {
					addLabel.setText("Success!");
				}
				else if (result == 1) {
					addLabel.setText("Error: Max fingerprint capacity reached");
				}
				else if (result == 2) {
					addLabel.setText("Error: User ID number already in use!");
				}
				else {
					addLabel.setText("ERROR!");
				}
				return;
			}			
		};
		add.addActionListener(addActionListener);
		addFingerPanel = new JPanel(new GridBagLayout());
		TitledBorder border = new TitledBorder("Register a Fingerprint");
		border.setTitleJustification(TitledBorder.LEFT);
		border.setTitlePosition(TitledBorder.TOP);
		addFingerPanel.setBorder(border);
		GridBagConstraints c = new GridBagConstraints();
		c.fill = GridBagConstraints.HORIZONTAL;
		c.ipadx = 100;
		c.ipady = 10;
		c.insets = new Insets(5,5,5,40);
		c.gridx = 0;
		c.gridy = 0;
		c.anchor = GridBagConstraints.CENTER;
		c.weightx = 0.2;
		addFingerPanel.add(fingerId, c);
		c.insets = new Insets(5,5,5,5);
		c.ipadx = 10;
		c.gridx = 1;
		addFingerPanel.add(add, c);
		c.gridx = 0;
		c.gridy = 1;
		c.gridwidth = 2;
		c.ipadx = 200;
		addFingerPanel.add(addLabel, c);
		addFingerPanel.setAlignmentX(Component.CENTER_ALIGNMENT);
		this.add(addFingerPanel);
	}
	
	private void createSelectPanel() {
		//create the port selector
		ports = new JComboBox<String>(logic.ports);
		ActionListener portActionListener = new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent event) {
				portName = (String) ports.getSelectedItem();
				logic.select(portName);
			}
		};
		ports.addActionListener(portActionListener);
		//create port refresh button
		JButton refresh = new JButton("Refresh");
		ActionListener refreshActionListener = new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				logic.refresh();
				ports.removeAllItems();
				for (String s : logic.ports) {
					ports.addItem(s);
				}
			}			
		};
		refresh.addActionListener(refreshActionListener);
		selectPanel = new JPanel(new GridBagLayout());
		TitledBorder border = new TitledBorder("Select a Serial Port");
		border.setTitleJustification(TitledBorder.LEFT);
		border.setTitlePosition(TitledBorder.TOP);
		selectPanel.setBorder(border);
		GridBagConstraints c = new GridBagConstraints();
		c.fill = GridBagConstraints.HORIZONTAL;
		c.ipadx = 150;
		c.ipady = 10;
		c.insets = new Insets(5,5,5,40);
		c.gridx = 0;
		c.gridy = 0;
		c.anchor = GridBagConstraints.CENTER;
		c.weightx = 0.2;
		selectPanel.add(ports, c);
		c.insets = new Insets(5,5,5,5);
		c.ipadx = 10;
		c.gridx = 1;
		selectPanel.add(refresh, c);
		selectPanel.setAlignmentX(Component.CENTER_ALIGNMENT);
		this.add(selectPanel);
	}
}