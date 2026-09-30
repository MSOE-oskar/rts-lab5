/**
 * @file NetworkCfg.h
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
 * This file defines a debug library for network config.  It will allow you to print to the console network messages and the like.  It can easily be conditionally compiled out.
 */

#include "debug_cfg.h"
#include "NetworkDebugLibrary.h"
#include "CommandQueue.h"
#include <iostream>

/**
 * This method will print a network message out to the console.  The code will only work if NETWORK_DEBUG is set to 1.
 * @param messageToPrint This si the message to print out.
 */
void printNetworkMessageToConsole(struct networkMessageStruct *messageToPrint) {
#if NETWORK_DEBUG_LIBRARY_ENABLED != 0
	std::cout << std::hex <<  "Security Mode: " << messageToPrint->securityMode << std::endl;
	std::cout << "Message id: " << messageToPrint->messageID << std::endl;
	std::cout << "Timestamp High: " << messageToPrint->timestampHigh << std::endl;
	std::cout << "Timestamp Low: " << messageToPrint->timestampLow << std::endl;
	std::cout << "Message Type: " << messageToPrint->messageType << std::endl;
	std::cout << "Destination: " << messageToPrint->messageDestination << std::endl;
	std::cout << "Message: " << messageToPrint->message << std::endl;
	std::cout << "Parameter 1: " << messageToPrint->parameter1 << std::endl;
	std::cout << "Parameter 2: " << messageToPrint->parameter2 << std::endl;
	std::cout << "Checksum: " << messageToPrint->xorChecksum << std::endl;
	std::cout << std::dec << std::endl;
#endif

}

/**
 * This method will print a queue entry out to the console.    The code will only work if NETWORK_DEBUG is set to 1.
 * @param entryToPrint This is the queued event to print out.
 */
void printQueueEntry(struct SWE4211RPi::CommandQueueEntry *entryToPrint) {
#if NETWORK_DEBUG_LIBRARY_ENABLED != 0
	std::cout << std::hex <<  "Command Type: " << entryToPrint->commandType << std::endl;
	std::cout << "Command: " << entryToPrint->command << std::endl;
	std::cout << "Parameter 1: " << entryToPrint->parameter1 << std::endl;
	std::cout << "Parameter 2: " << entryToPrint->parameter2 << std::endl;
	std::cout << std::dec << std::endl;
#endif
}

