import com.fazecast.jSerialComm.*;

public class YekSerial {
	//0 (ascii 48) - addacc - name of account - up to 120 bytes, receive a 0 on confirmation, nonzero on error
	//			send username - up to 120 bytes, receive 0 on confirmation, nonzero on error
	//			send pw - 0 for confirmation. nonzero on error
	//1 - remove acc - name of account - up to 120 bytes (-1) to delete all accounts
	//2 - add finger - id number of finger 2 bytes
	//3 - remove finger - id number of finger 2 bytes or send 255(-1) to delete all fingerprints
	//4 - flip the screen
	private static final int SLEEP_COUNT = 1500;
	private SerialPort[] portList;
	public String[] ports;
	private SerialPort port;
	
	public YekSerial(){
		//initialize stuff
		this.refresh();
		port = null;
	}
	
	//selects a given port
	public void select(String name) {
		port = SerialPort.getCommPort(name);
		//9600 baud, 8 data bits, one stop bit, no parity
		port.openPort();
		port.setComPortParameters(9600, 8, SerialPort.ONE_STOP_BIT, SerialPort.NO_PARITY);
	}
	
	//refreshes the list of serial Ports
	public void refresh() {
		portList = SerialPort.getCommPorts();
		ports = new String[portList.length];
		for (int i = 0; i < portList.length; i++) {
			ports[i] = portList[i].getSystemPortName();
		}
	}
	
	//tell us whether or not yek is connected
	public boolean connected() {
		return port.isOpen();
	}
	
	//flips the screen orientation of the Yek
	public void flipScreen() {
		if (!this.connected()) return;
		byte [] writeBuf = {52};
		port.writeBytes(writeBuf, 1);
		return;
	}
	
	//register a new user
	public void register_user() {
		if (!this.connected()) return;
		//write it
		byte[] writeBuf = {50};
		port.writeBytes(writeBuf, 1);
		return;
	}
	
	//deletes all users
	public void delete_user() {
		if (!this.connected()) return;
		byte[] writeBuf = {51};
		port.writeBytes(writeBuf, 1);
		return;
	}
	
	//add an account
	public void add_acc(String name, String id, String pw) {
		if (!this.connected()) return;
		byte[] writeBuf = {48};
		port.writeBytes(writeBuf, 1);
		try {
			Thread.sleep(SLEEP_COUNT);
		} catch (InterruptedException e) {
			return; //shit happened while sleeping
		} //sleep for a second
		port.writeBytes(name.getBytes(), name.length());
		try {
			Thread.sleep(SLEEP_COUNT);
		} catch (InterruptedException e) {
			return;
		}
		//write the username
		port.writeBytes(id.getBytes(), id.length());
		try {
			Thread.sleep(SLEEP_COUNT);
		} catch (InterruptedException e) {
			return;
		}
		//write the pw
		port.writeBytes(pw.getBytes(), pw.length());
		return;
	}
	
	//delete account
	public void del_acc(String name) {
		if (!this.connected()) return;
		byte[] writeBuf = {49};
		port.writeBytes(writeBuf, 1);
		try {
			Thread.sleep(SLEEP_COUNT);
		} catch (InterruptedException e) {
			return; //shit happened while sleeping
		} //sleep for a second
		port.writeBytes(name.getBytes(), name.length());
	}
}