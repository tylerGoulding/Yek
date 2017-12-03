import com.fazecast.jSerialComm.*;

public class YekSerial {
	//0 - addacc - name of account - up to 120 bytes, receive a 0 on confirmation, nonzero on error
	//			send username - up to 120 bytes, receive 0 on confirmation, nonzero on error
	//			send pw - 0 for confirmation. nonzero on error
	//1 - remove acc - name of account - up to 120 bytes (-1) to delete all accounts
	//2 - add finger - id number of finger 2 bytes
	//3 - remove finger - id number of finger 2 bytes or send 255(-1) to delete all fingerprints
	//0 on success
	//nonzero on error
	
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
		try {
			port.closePort();
		} 
		finally {
			port = SerialPort.getCommPort(name);
			//9600 baud, 8 data bits, one stop bit, no parity
			port.openPort();
			port.setComPortParameters(9600, 8, SerialPort.ONE_STOP_BIT, SerialPort.NO_PARITY);
		}
	}
	
	//refreshes the list of serial Ports
	public void refresh() {
		portList = SerialPort.getCommPorts();
		ports = new String[portList.length];
		for (int i = 0; i < portList.length; i++) {
			ports[i] = portList[i].getDescriptivePortName();
		}
	}
	
	//tell us whether or not yek is connected
	public boolean connected() {
		return port.isOpen();
	}
	
	//register a new user, returns 
	//0 on success
	//1 if that id is already registered
	//2 if other failure(not connected or some shit)
	public int register_user(int id) {
		if (!this.connected()) return 2;
		//write it
		byte[] writeBuf = {2, (byte) id};
		int i = port.writeBytes(writeBuf, 2);
		if (i != 2) return 2; //error writing
		byte[] readBuf = new byte[1];
		i = port.readBytes(readBuf, 1);
		if (i != 1) return 2; //error reading
		return (int) readBuf[0];
	}
	
	//returns 0 on success
	//returns 1 if no user for that id
	//returns 2 if other failure(not connected or some shit)
	public int delete_user(int id) {
		if (!this.connected()) return 2;
		byte[] writeBuf = {3, (byte) id};
		int i = port.writeBytes(writeBuf, 2);
		if (i != 2) return 2; //error writing
		byte[] readBuf = new byte[1];
		i = port.readBytes(readBuf, 1);
		if (i != 1) return 2; //error reading
		return (int) readBuf[0];
	}
	
	//0 on success
	//1 on error, memory full
	//2 for other error(not connected or something)
	public int add_acc(String name, String id, String pw) {
		if (this.connected()) return 2;
		int len = name.length() + 1;
		byte[] a = {1};
		byte[] writeBuf = new byte[len];
		System.arraycopy(a, 0, writeBuf, 0, 1);
		System.arraycopy(name.getBytes(), 0, writeBuf, 1, len-1);
		int i = port.writeBytes(writeBuf, len);
		if (i != len) return 2; //error writing
		byte[] readBuf = new byte[1];
		i = port.readBytes(readBuf, 1);
		if (i != 1) return 2; //error reading
		if (readBuf[0] != 0) return 2; //error on yek side
		//write the username
		i = port.writeBytes(id.getBytes(), id.length());
		if (i != id.length()) return 2; //error writing
		i = port.readBytes(readBuf, 1);
		if (i != 1) return 2; //error reading
		if (readBuf[0] != 0) return 2; //error on yek side
		//write the pw
		i = port.writeBytes(id.getBytes(), id.length());
		if (i != id.length()) return 2; //error writing
		i = port.readBytes(readBuf, 1);
		if (i != 1) return 2; //error reading
		return (int) readBuf[0];
	}
	
	//0 on success
	//1 on error- no such account by that name
	//2 on other error(not connected or something
	public int del_acc(String name) {
		if (this.connected()) return 2;
		int len = name.length() + 1;
		byte[] a = {1};
		byte[] writeBuf = new byte[len];
		System.arraycopy(a, 0, writeBuf, 0, 1);
		System.arraycopy(name.getBytes(), 0, writeBuf, 1, len-1);
		int i = port.writeBytes(writeBuf, len);
		if (i != len) return 2; //error writing
		byte[] readBuf = new byte[1];
		i = port.readBytes(readBuf, 1);
		if (i != 1) return 2; //error reading
		return (int) readBuf[0];
	}
}