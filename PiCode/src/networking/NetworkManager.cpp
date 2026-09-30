/**
 * @file NetworkManager.cpp
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
 * This file defines the implementation for the Network Manager.  The Network Manager manages network connections and acts as a server, receiving messages sent over a socket.
 */

#include "NetworkManager.h"
#include "NetworkCfg.h"
#include "CommandQueue.h"
#include "NetworkMessage.h"
#include "NetworkCommands.h"
#include "NetworkDebugLibrary.h"
#include <errno.h>       // errno, EINTR
#include <stdio.h>       // perror, fprintf
#include <string.h>      // memset
#include <unistd.h>
#include <sys/types.h>   // ssize_t
#include <sys/socket.h>
#include <netinet/in.h>
#include <string>
