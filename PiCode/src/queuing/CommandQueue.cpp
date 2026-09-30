/**
 * @file CommandQueue.cpp.cpp
 * @author  Walter Schilling (schilling@msoe.edu)
 * @version 1.0
 *
 * @section LICENSE
 *
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
 * Implementation of the CommandQueue class.
 * The CommandQueue is a thread-safe, first-in first-out (FIFO) queue.  One or more threads
 * (producers) enqueue commands for a device, and another thread (the consumer) dequeues and
 * processes them.  Each command is stored in a CommandQueueEntry structure.
 * Two synchronization objects work together to make the queue safe to share between threads:
 * a mutex, which ensures that only one thread at a time modifies the underlying queue, and a
 * counting semaphore, which tracks how many items are waiting so that a consumer can block
 * until work is available.
 */

#include "CommandQueue.h"
#include <semaphore.h>
