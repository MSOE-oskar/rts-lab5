/**
 * @file CommandQueue.h
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
 * Function prototypes and class definitions for the command queue class.
 * This class, the CommandQueue, allows a user to enque a set of commands for a device. The commands must be sinple integers,
 * but they can have bitmapped representations.  The queue can hold as many items as is necessary.
 *
 * @author Walter Schilling (schilling@msoe.edu)
 * @bug No known bugs.
 */

#ifndef COMMANDQUEUE_H_
#define COMMANDQUEUE_H_
#include <stdint.h>
#include <queue>        /* Required for the queue data structure.*/
#include <mutex>        /* Required for locking critical sections. */
#include <semaphore.h>  /* required for semaphores */

namespace SWE4211RPi {

/**
 * This structure defines a command queue entry.  This is what can be enqueued onto this special queue.
 */
struct CommandQueueEntry
{
/**
 * The command type is the type of command.  For basic usage this is always 0.  For messages which are broadcast going to all queues, it is 0x7FFFFFFF.
 */
	uint32_t commandType;
	/**
	 * This is the command that is being enqueued.  Different commands have different values.
	 */
	uint32_t command;
	/**
	 * This is the first parameter for the command.  Many commands do not have parameters.
	 */
	uint32_t parameter1;
	/**
	 * This is the second parameter for the command.  Again, it is rare to use this parameter.
	 */
	uint32_t parameter2;
};

class CommandQueue {
private:
	/**
	 * This attribute holds the contents of the queue.  Items are processed in FIFO order.  The internal parameters of this attribute should be used to determine if the queue is empty or not.
	 */
	std::queue<CommandQueueEntry> commandQueueContents;
	/**
	 * This is a counting semaphore which keeps track of how many items are on the queue.  It should only be used as a pure semaphore.
	 */
	sem_t queueCountSemaphore;
	/**
	 * This is a mutex for the class which is used to lock critical sections in different methods as is applicable.
	 */
	std::mutex queueMutex;

public:
	/**
	 * This is the default constructor, which creates an instance of the queue.
	 */
	CommandQueue();

	/**
	 * The virtual destructor, which deallocates any allocated resources.
	 */
	virtual ~CommandQueue();

	/**
	 * This method will indicate whether or not the queue has an item that is ready to be dequeued.
	 * @return true if there is an item on the queue.  False otherwise.
	 */
	bool hasItem();

	/**
	 * This method will dequeue the next command from the queue.  This method will block if there are no items on the queue.
	 * @return The return will be the next command that is to be processed.
	 */
	CommandQueueEntry dequeue();

	/**
	 * This method will enqueue a command on the queue.  Any number can be enqueued as a command.
	 * Enqueueing a command will cause a thread blocked waiting for a command to be unblocked.
	 * @param command This is the command that is to be enqueued.
	 */
	void enqueue(CommandQueueEntry value);
};

}
#endif /* COMMANDQUEUE_H_ */
