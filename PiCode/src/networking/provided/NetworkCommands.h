/**
 * @file NetworkCommands.h
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
 * This file defines the commands that are to be sent over the network to the device.
 */

#ifndef NETWORKCOMMANDS_H_
#define NETWORKCOMMANDS_H_

/**
 * The command to turn a light on in a blinking state.
 */
#define LIGHTPWMADJUSTMENTCMD   (0x01000000)
// Parameter 1: This is the PWM duty cycl.e  Valid range is 0 to 1000, with 1000 representing 100% duty cycle.

/**
 * The command to simply turn a light on.
 */
#define LIGHTONCMD      (0x00100000)

/**
 * The command to turn a light off.
 */
#define LIGHTOFFCMD  (0x00010000)

/**
 * The reporting of a new IP address.
 */
#define REPORT_NEW_IPADDRESS (0x08000011) // This command will indicate a new IP address has been received.
	// Parameter 1: This is the 32 bit ip address that the machine is connected to.


#endif /* NETWORKCOMMANDS_H_ */
