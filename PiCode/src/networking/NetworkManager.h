/**
 * @file NetworkManager.h
 * @author  Walter Schilling (schilling@msoe.edu)
 * @version 1.0
 *
 * @section LICENSE
 *
 * This code is developed as part of the MSOE SWE4211 Real Time Systems course,
 * but can be freely used by others.
 *
 * SWE4211 Real Time Systems is a required course for students studying the
 * discipline of software engineering.
 *
 * This Software is provided under the License on an "AS IS" basis and
 * without warranties of any kind concerning the Software, including
 * without limitation merchantability, fitness for a particular purpose,
 * absence of defects or errors, accuracy, and non-infringement of
 * intellectual property rights other than copyright. This disclaimer
 * of warranty is an essential part of the License and a condition for
 * the grant of any rights to this Software.
 *
 * @section DESCRIPTION
 * This file defines the interface for the Network Manager.  The Network Manager manages
 * network connections and acts as a server, receiving messages sent over a socket to the class.
 */

#ifndef NETWORKMANAGER_H_
#define NETWORKMANAGER_H_

#include <netinet/in.h>
#include "CommandQueue.h"
#include "RunnableClass.h"
#include "NetworkCfg.h"
#include "NetworkMessage.h"
#include <string>

namespace SWE4211RPi {

class NetworkManager: public RunnableClass {
private:
	/**
	 * This is the port that will be used for receiving messages over the network.
	 */
	unsigned short portNumber;

	/**
	 * This is a pointer to the array of queues that will be used to communicate with other devices.
	 */
	CommandQueue **referencequeues;

	/**
	 * This integer is the file descriptor for the socket that is going to be listened to for connections.
	 */
	int server_fd = -1;

	/**
	 * This is the connected socket of the network manager.  This will be updated whenever a connection is made and
	 * it is used to send messages back to the master as well as to receive messages from the master.
	 */
	int connectedSocket = -1;

public:
	/**
	 * This is the constructor for the Network Manager.  It will instantiate a new instance of the class.
	 * @param port This is the port that the network manager is to listen on for incoming connections.
	 * @param queues This is the array of pointers to queues that is to be used for enqueueing received requests.
	 * @param threadName This is the name given to the executing thread.  It is a simple, string that is human readable.
	 */
	NetworkManager(unsigned short port, CommandQueue *queues[], std::string threadName);

	/**
	 * This is the destructor for the class which will clean up from the instantiation and operation of the class.
	 */
	virtual ~NetworkManager();

	/**
	 * This is the run method for the class.  It contains the code that is to run periodically on the given thread.
	 */
	void run();

	/**
	 * This method will override the default stop method.  In doing so, it must call the base class's stop method prior to invoking it's own logic.
	 */
	void stopThreadExecution();

	/**
	 * This method will return the socket ID for the given class.
	 * @return The socket ID will be returned.
	 */
	int getSocketID();

protected:
	/**
	 * This method receives one complete networkMessageStruct from the connected client.
	 * @param message This is where the received message is stored.  The fields are left in network byte order.
	 * @return true if a complete message was received.  false if the client closed the connection,
	 * only part of a message arrived, or an error occurred.
	 */
	bool receiveNetworkMessage(networkMessageStruct &message);

	/**
	 * This method will process a received network message.  When it does this, it will calculate out and verify the checksum.  If the checksum is correct, the contents will then be enqueued to the correct queue.
	 * @param receivedMessage This is a reference to the received message structure.
	 * @return The calculated checksum is returned.
	 */
	int processReceivedMessage(networkMessageStruct &receivedMessage);

	/**
	 * This method will report across any queue whenever a new connection is made to this software.
	 * @param clientAddress This is the ip address of the client device represented as a single 32 bit number.  if there is no connection, the value will be 0.
	 */
	void reportNewClientConnection(const struct sockaddr_in &clientAddress);
};

}

#endif /* NETWORKMANAGER_H_ */
