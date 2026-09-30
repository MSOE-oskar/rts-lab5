/*
 * NetworkCommandTypes.h
 *
 *  Created on: Dec 5, 2024
 *      Author: swestudent
 */

#ifndef NETWORKING_NETWORKCOMMANDTYPES_H_
#define NETWORKING_NETWORKCOMMANDTYPES_H_

/**
 * This indicates that we have a command being sent.  A command type indicates that a device is being commanded to perform some operation.
 */
#define COMMAND_MSG_TYPE (0x00000100)
/**
 * This indicates that we have a status report.  A status report is sent from a device to another device.
 */
#define STATUS_REPORT_MSG_TYPE (0x00000200)

/**
 * A heartbeat message indicates a request or response is being sent to see if a given device is operating properly.
 */
#define HEARTBEAT_MSG_TYPE (0x5AA55AA5)

/**
 * A broadcast message is sent to all devices / components within the system.
 */
#define BROADCAST_MESSAGE_TYPE (0x7FFFFFFF)

#endif /* NETWORKING_NETWORKCOMMANDTYPES_H_ */
