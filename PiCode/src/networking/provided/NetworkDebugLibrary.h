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
 * This file defines a debug library for network config.
 */
#ifndef NETWORKDEBUGLIBRARY_H_
#define NETWORKDEBUGLIBRARY_H_

#include "NetworkMessage.h"
#include "CommandQueue.h"
/**
 * This method will print a network message out to the console.  The code will only work if NETWORK_DEBUG is set to 1.
 * @param messageToPrint This is the message to print out.
 */
void printNetworkMessageToConsole(struct networkMessageStruct *messageToPrint);

/**
 * This method will print a queue entry out to the console.    The code will only work if NETWORK_DEBUG is set to 1.
 * @param entryToPrint This is the queued event to print out.
 */
void printQueueEntry(struct SWE4211RPi::CommandQueueEntry *entryToPrint);



#endif /* NETWORKDEBUGLIBRARY_H_ */
