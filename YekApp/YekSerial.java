import java.io.IOException;
import java.io.OutputStream;

import com.fazecast.jSerialComm.*;

public class YekSerial {
	//48 - addacc - name of account - up to 120 bytes, receive a 0 on confirmation, nonzero on error
	//			send username - up to 120 bytes, receive 0 on confirmation, nonzero on error
	//			send pw - 0 for confirmation. nonzero on error
	//49 - remove acc - name of account - up to 120 bytes (-1) to delete all accounts
	//50 - add finger - id number of finger 2 bytes
	//51 - remove finger - id number of finger 2 bytes or send 255(-1) to delete all fingerprints
	//52 to flip the screen
	//48 on success
	//nonzero on error
	private static final int SLEEP_COUNT = 1500;
	private SerialPort[] portList;
	public String[] ports;
	private SerialPort port;
	
	public YekSerial(){
		//initialize stuff
		this.refresh();
		port = null;
	}
	
	public void select(String name) {
		//close the current port
		//if (port == null) port.closePort();
		port = SerialPort.getCommPort(name);
		//port = portList[0];
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
	
	public void flipScreen() {
		if (!this.connected()) return;
		byte [] writeBuf = {52};
		port.writeBytes(writeBuf, 1);
		return;
	}
	
	//register a new user, returns 
	//0 on success
	//1 if that id is already registered
	//2 if other failure(not connected or some shit)
	public int register_user() {
		if (!this.connected()) return 1;
		//write it
		byte[] writeBuf = {50};
		int i = port.writeBytes(writeBuf, 1);
		if (i != 2) return 2; //error writing
		return 0;
	}
	
	//returns 0 on success
	//returns 2 if other failure(not connected or some shit)
	public int delete_user() {
		if (!this.connected()) return 1;
		byte[] writeBuf = {51};
		int i = port.writeBytes(writeBuf, 1);
		if (i != 1) return 2; //error writing
		return 0;
	}
	
	//0 on success
	//1 on error, memory full
	//2 for other error(not connected or something)
	public int add_acc(String name, String id, String pw) {
		if (!this.connected()) return 1;
		byte[] writeBuf = {48};
		int i = port.writeBytes(writeBuf, 1);
		if (i != 1) return 2;
		try {
			Thread.sleep(SLEEP_COUNT);
		} catch (InterruptedException e) {
			return 3; //shit happened while sleeping
		} //sleep for a second
		i = port.writeBytes(name.getBytes(), name.length());
		if (i != name.length()) return 4; 
		//byte[] readBuf = new byte[1];
		try {
			Thread.sleep(SLEEP_COUNT);
		} catch (InterruptedException e) {
			return 3; //shit happened while sleeping
		}
		//write the username
		i = port.writeBytes(id.getBytes(), id.length());
		if (i != id.length()) return 7; //error writing
		try {
			Thread.sleep(SLEEP_COUNT);
		} catch (InterruptedException e) {
			return 3; //shit happened while sleeping
		}
		//write the pw
		i = port.writeBytes(pw.getBytes(), pw.length());
		if (i != pw.length()) return 10; //error writing
		return 0;
	}
	
	//0 on success
	//1 on error- no such account by that name
	//2 on other error(not connected or something
	public int del_acc(String name) {
		if (!this.connected()) return 1;
		byte[] writeBuf = {49};
		int i = port.writeBytes(writeBuf, 1);
		if (i != 1) return 2;
		try {
			Thread.sleep(SLEEP_COUNT);
		} catch (InterruptedException e) {
			return 3; //shit happened while sleeping
		} //sleep for a second
		i = port.writeBytes(name.getBytes(), name.length());
		if (i != name.length()) return 3;
		return 0;
	}
}