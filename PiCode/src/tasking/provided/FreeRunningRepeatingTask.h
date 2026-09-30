/**
 * @file FreeRunningRepeatingTask.h
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
 *      This file defines the behavior for a repeating task, which is similar to a periodic task, but is controlled by an external entity.
 *      For example, a camera which returns a new image every 1/30th of a second would use this class, as the hardware controls the rate,
 *      not the software.
 */

#ifndef FREERUNNINGREPEATINGTASK_H_
#define FREERUNNINGREPEATINGTASK_H_

#include "RunnableClass.h"

#include <chrono>

namespace SWE4211RPi {


class FreeRunningRepeatingTask : public RunnableClass {
private:
	/**
	 * This variable stores the worst case execution time from a CPU time standpoint.
	 */
	long worstCaseExecutionTime = 0;

	/**
	 * This variable holds the last execution value for the CPU, showing the time it took to execute.
	 */
	long lastExecutionTime = 0;

	/**
	 * This variable will hold the count as to how many times the deadline was missed for the given system.
	 */
	long missedDeadlineCount = 0;

	/**
	 * This variable holds the last wall time measurement for the given periodic task.  The wall time is the time from the start to the end of the task running.
	 */
	long lastWallTime = 0;

	/**
	 * This variable holds the worst case wall time.  This is the worst case from start to finish of the given periodic task.
	 */
	long worstCaseWallTime = 0;

	/**
	 * This is a private method that will be used by start to invoke the run method.
	 */
	void invokeRun();

public:
	/**
	 * This is the default constructor for the class.
	 * @param threadName This is the name of the thread in a human readable format.
	 * @param priority This is the priority for the task.
	 */
	FreeRunningRepeatingTask(std::string threadName, int priority=1);

	/**
	 * This method will clean up from the periodic task and its execution.
	 */
	virtual ~FreeRunningRepeatingTask();

public:

	/**
	 * This is the task method.  The task method will be invoked periodically every taskPeriod
	 * units of time, where the period is set by an existing piece of hardware.
	 */
	virtual void taskMethod()=0;

	/**
	 * This is the run method for the class.
	 */
	void run() final;

	/**
	 * This method will print out information about the given thread.  The info will be dependent upon the given thread.
	 */
	virtual void printInformation();

	/**
	 * This method will obtain the CPU usage.  CPU usage is the worst case measured execution time divided by the period.
	 * @return The CPU usage will be returned.
	 */
	virtual double getCPUUsageInfo();

	/**
	 * This method will reset thread diagnostics back to their default values.  The wall times and CPU times will be set to 0.
	 */
	virtual void resetThreadDiagnostics();

protected:
	/**
	 * This method will start up any runnable objects which are contained within a class that implements the RUnnable interface.
	 * If there are no other objects that are runnable, there is no need to override this method.  However, if a child class
	 * contains an instance of a runnable object, then this method must be overridden in the derived class.
	 *
	 */
	virtual void startChildRunnables() final;


	/**
	 * This method will stop any runnable objects which are contained within a class that implements the Runnable interface.
	 * If there are no other objects that are runnable, there is no need to override this method.  However, if a child class
	 * contains an instance of a runnable object, then this method must be overridden in the derived class.
	 */
	virtual void stopChildRunnables() final;

	/**
	 * This method will start up any runnable objects which are contained within a class that implements the RUnnable interface.
	 * If there are no other objects that are runnable, there is no need to override this method.  However, if a child class
	 * contains an instance of a runnable object, then this method must be overridden in the derived class.
	 *
	 */
	virtual void startChildTasks();

	/**
	 * This method will stop any runnable objects which are contained within a class that implements the RUnnable interface.
	 * If there are no other objects that are runnable, there is no need to override this method.  However, if a child class
	 * contains an instance of a runnable object, then this method must be overridden in the derived class.
	 */
	virtual void stopChildTasks();
};

}
#endif /* FREERUNNINGREPEATINGTASK_H_ */


