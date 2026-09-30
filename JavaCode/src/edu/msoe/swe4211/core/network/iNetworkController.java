package edu.msoe.swe4211.core.network;

import java.io.IOException;
import java.net.UnknownHostException;

/**
 * This interface defines a network controller. The network controller will
 * allow the user to communicate with a single Raspberry PI instance and send
 * commands to it.
 *
 * @author wws
 */
public interface iNetworkController {
    public static final int COMMAND_MSG_TYPE = 0x00000100;

    /**
     * This method will set the IP address of the raspberry pi.
     *
     * @param ip The ip address in the form AAA.BBB.CCC.DDD
     */
    public void setIPAddress(String ip);

    /**
     * This method will connect to the given IP address.
     *
     * @throws UnknownHostException This will be thrown if no connection can be made with the host.
     * @throws IOException          This exception will be thrown if IO can not be connected with.
     */
    public void connect() throws UnknownHostException, IOException;

    /**
     * This method will disconnect from the given networked machine.
     */
    public void disconnect();

    /**
     * This method will return the connection status.
     *
     * @return true if connected. False OTW.
     */
    public boolean getConnectionStatus();

    /**
     * This method will send a single 32 bit message of a given type to the remote machine.
     *
     * @param destination This is the destination for the given message.
     * @param message     This is the message to send.
     * @throws IOException This exception will be thrown if there is an error.
     */
    boolean sendMessage(int destination, int message) throws IOException;

    /**
     * This method will send a single 32 bit message of a given type to the remote machine.
     * @param destination This is the destination for the given message.
     * @param messageType This is the type of message that is to be sent.
     * @param message     This is the message to send.     *
     * @param parameter1  This is the first parameter associated with the message.
     * @param parameter2  This is the second parameter associated with the message.
     * @return
     * @throws IOException This exception will be thrown if there is an error.
     */
    boolean sendMessage(int destination, int messageType, int message, int parameter1, int parameter2) throws IOException;

    /**
     * This method will send a single 32 bit message of a given type to the remote machine.
     *
     * @param destination       This is the destination for the given message.
     * @param messageType       This is the type of message that is to be sent.
     * @param message           This is the message to send.
     * @param forcedXORChecksum This is the forced checksum to use for the message.  Likely it will not be correct.
     * @throws IOException This exception will be thrown if there is an error.
     */
    public boolean sendMessage(int destination, int messageType, int message, int parameter1, int parameter2, int forcedXORChecksum) throws IOException;


    /**
     * This method will receive a message that is sent across the network to the JVM.
     *
     * @param length This is the number of bytes that are to be received.
     * @return The return will be an array of ints representing the bytes that were sent.
     * @throws IOException This will be thrown if there
     */
    int[] receiveMessage(int length) throws IOException;

    /**
     * This method will subscribe a new observer to this network controller.
     *
     * @param newObserver This is the observer that is to be subscribed.
     */
    public void subscribe(iNetworkControllerStatusObserver newObserver);
}
