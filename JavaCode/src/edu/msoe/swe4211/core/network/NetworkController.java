package edu.msoe.swe4211.core.network;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.Socket;
import java.net.UnknownHostException;
import java.util.LinkedList;
import java.util.List;

/**
 * This class will provide implementation for the network controller.
 *
 * @author wws
 */
public class NetworkController implements iNetworkController {
    /**
     * This is the socket that is to be used for communications.
     */
    private Socket socket;
    /**
     * This is the ip address of the given machine.
     */
    private String ip;
    /**
     * This is the data stream that is to be written to when communicating with the device.
     */
    private DataOutputStream out;
	private DataInputStream in;
    /**
     * This boolean will indicate whether or not there is a current valid connection.
     */
    private boolean connected = false;
	private int messageSequenceNumber=0;
	private long timestamp=0;

	private List<iNetworkControllerStatusObserver> observers = new LinkedList();

	private static final int COMMAND_MSG_TYPE = 0x09;
	
	public NetworkController() {
		super();
	}

	@Override
	public void setIPAddress(String ip) {
		this.ip = ip;
	}

	@Override
	public void connect() throws UnknownHostException, IOException {
		socket = new Socket(ip, 9090);
		out = new DataOutputStream(socket.getOutputStream());
		in = new DataInputStream(socket.getInputStream());
		connected = true;
		for (iNetworkControllerStatusObserver obs : this.observers)
		{
			obs.updateConnectionStatus(connected);
		}
	}

	@Override
	public void disconnect() {
		try {
			socket.close();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		connected = false;
		out = null;
		for (iNetworkControllerStatusObserver obs : this.observers)
		{
			obs.updateConnectionStatus(connected);
		}
	}

	@Override
	public boolean getConnectionStatus() {
		return connected;
	}



	@Override
	public boolean sendMessage(int destination, int message) throws IOException {
		boolean retVal = false;
		if (out != null) {
			retVal = sendMessage(destination, COMMAND_MSG_TYPE, message, 0, 0);
		}
		return retVal;
	}

	@Override
	public boolean sendMessage(int destination, int messageType, int message, int parameter1, int parameter2) throws IOException {
		boolean retVal = false;
		if (out != null) {
			int encryptionType = 0;

			// The msg sequence number is a number that keeps increasing each time a message is sent.
			int msgID = messageSequenceNumber++;
			long timestamp = System.currentTimeMillis();
			int timestampHigh = ((int)((timestamp & 0xFFFFFFFF00000000l)>> 32));
			int timestampLow = ((int)((timestamp & 0x00000000FFFFFFFFl)));

			int checksum = msgID ^ timestampHigh ^ timestampLow ^ messageType ^ message ^ parameter1 ^ parameter2 ^destination;
			System.out.printf("Sent: MID %x TS %x %x T %x D %x MSG %x CK %x\n", msgID, timestampHigh, timestampLow, messageType, destination, message, checksum);
			out.writeInt(encryptionType);
			out.writeInt(msgID);
			out.writeInt(timestampHigh);
			out.writeInt(timestampLow);
			out.writeInt(messageType);
			out.writeInt(destination);
			out.writeInt(message);
			out.writeInt(parameter1);
			out.writeInt(parameter2);
			out.writeInt(checksum);
			out.flush();
			retVal = true;
		}
		return retVal;
	}

	@Override
	public boolean sendMessage(int destination, int messageType, int message, int parameter1, int parameter2, int forcedXORChecksum) throws IOException {
		boolean retVal = false;
		if (out != null) {
			int encryptionType = 0;

			// The msg sequence number is a number that keeps increasing each time a message is sent.
			int msgID = messageSequenceNumber++;
			long timestamp = System.currentTimeMillis();
			int timestampHigh = ((int)((timestamp & 0xFFFFFFFF00000000l)>> 32));
			int timestampLow = ((int)((timestamp & 0x00000000FFFFFFFFl)));

			System.out.printf("Sent: MID %x TS %x %x T %x D %x MSG %x CK %x\n", msgID, timestampHigh, timestampLow, messageType, destination, message, forcedXORChecksum);
			out.writeInt(encryptionType);
			out.writeInt(msgID);
			out.writeInt(timestampHigh);
			out.writeInt(timestampLow);
			out.writeInt(messageType);
			out.writeInt(destination);
			out.writeInt(message);
			out.writeInt(parameter1);
			out.writeInt(parameter2);

			out.writeInt(forcedXORChecksum);
			out.flush();
			retVal = true;
		}
		return retVal;
	}

	@Override
	public int[] receiveMessage(int numberOfBytes)  throws IOException {
		int[] retVal = new int[numberOfBytes];
		if (in != null)
		{
			boolean valid = false;

			int[] valuesRead = new int[numberOfBytes];
			for (int index = 0; index < numberOfBytes; index++)
			{
				valuesRead[index] = 0;
				retVal[index] = 0;
			}

			while (!valid) {
				// Move the values up by 1 location in the array.
				for(int index = 0; index< (numberOfBytes-1); index++)
				{
					valuesRead[index] = valuesRead[index+1];
				}

				// Read in an int.
				valuesRead[numberOfBytes-1] = in.readInt();


				int xorChecksum = 0;
				// Calculate the checksum and check it.
				for(int index = 0; index< (numberOfBytes-1); index++)
				{
					xorChecksum= xorChecksum ^ valuesRead[index];
				}

				// Verify the checksum and set return values if correct.
				if (xorChecksum == valuesRead[numberOfBytes-1]) {
					// Copy to the return array.
					for (int index = 0; index < numberOfBytes; index++)
					{
						retVal[index] = valuesRead[index];
					}
					valid = true;
				}
			}
		}
		return retVal;
	}

	@Override
	public void subscribe(iNetworkControllerStatusObserver newObserver) {
		observers.add(newObserver);
	}
}
