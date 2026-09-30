/**
 * @file PeriodicTask.cpp
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
 *      This file defines the behavior for a periodic task.  A periodic task is
 *      one in which the run method is called periodically by the task manager.
 */

#include "PeriodicTask.h"
#include <iostream>
#include <chrono>
#include <iomanip>

using namespace SWE4211RPi;

#define DELTA_TIMESTAMPS(endTS, startTS) ((endTS.tv_sec * 1000000 + endTS.tv_nsec / 1000) - (startTS.tv_sec * 1000000 + startTS.tv_nsec / 1000))


/**
 * This is the default constructor for the class.
 * @param threadName This is the name of the thread in a human readable format.
 * @param period This is the period for the task, given in microseconds.
 * Must be at least 100 microseconds.
 */
PeriodicTask::PeriodicTask(std::string threadName, uint32_t period,
		int priority) :
		RunnableClass(threadName, priority) {
	this->setTaskPeriod(period);
	this->setPriority(priority);
}

/**
 * This method will clean up from the periodic task and its execution.
 */
PeriodicTask::~PeriodicTask() {
	/**
	 * Nothing to be done in the destructor.
	 */
}

/**
 * This will set the period for the given task.
 * @param period This is the period, given in microseconds, between executions of this task.
 * However, it must be at least 100 microseconds (.1 ms)
 */
void PeriodicTask::setTaskPeriod(uint32_t period) {
	/**
	 * Set the task period to the parameter passed in if it is greater than or equal to 100.
	 */
	if (period >= 100) {
		taskPeriod = period;
	}
}

/**
 * This method will return the task period.
 * @return the Task period in microseconds will be returned.
 */
uint32_t PeriodicTask::getTaskPeriod() {
	return taskPeriod;
}

/**
 * This method will suspend execution until the next period has been reached.  It will do this by blocking.
 */
void PeriodicTask::waitForNextExecution(
		std::chrono::microseconds remainingSleepTime) {
	/**
	 * Sleep for the given amount of time.
	 */
	std::this_thread::sleep_for(remainingSleepTime);
}

/**
 * This method will suspend execution until the next period has been reached.  It will do this by blocking.
 */
void PeriodicTask::waitForNextExecution(struct timespec sleepTime)
{
	/**
	 * Sleep for the given amount of time.
	 */
	nanosleep(&sleepTime, NULL);
}

/**
 * This method will return the CPU usage for the given task.  The usage will be as a percentage value.
 */
double PeriodicTask::getCPUUsageInfo() {
	double cpuUsage = ((double) worstCaseExecutionTime / (double) taskPeriod)
			* 100.0;
	return cpuUsage;
}

/**
 * This method will print out information about the given thread.  The info will be dependent upon the given thread.
 */
void PeriodicTask::printInformation() {
	std::cout << myOSThreadID << "\t" << std::setw(23) << myName
			<< std::setw(5) << getPriority() << "\tPER\t " << std::setw(10)
			<< taskPeriod << "\t " << std::setw(12) << lastExecutionTime
			<< "\t " << std::setw(8) << worstCaseExecutionTime << "\t "
			<< std::setw(12) << lastWallTime << "\t " << std::setw(8)
			<< worstCaseWallTime << "\t" << std::fixed
			<< std::setprecision(3) << getCPUUsageInfo() << "%";
	if (worstCaseWallTime > taskPeriod) {
		std::cout << " **";
	}
	if (missedDeadlineCount > 0) {
		std::cout << missedDeadlineCount;
	}
	std::cout << "\n";
}

/**
 * This method will reset thread diagnostics back to their default values.
 */
void PeriodicTask::resetThreadDiagnostics() {
	// Reset all diagnostic variables to zero.
	worstCaseExecutionTime = 0;
	lastExecutionTime = 0;
	lastWallTime = 0;
	worstCaseWallTime = 0;
	missedDeadlineCount = 0;
}

/**
 * This is a private method that will be used by start to invoke the run method.
 */
void PeriodicTask::run() {
	/**
	 * Set keep going to be true, to indicate that the thread is to continue running.
	 */
	keepGoing = true;

	while (keepGoing == true) {
		// Get the start time for the given iteration of the task.
		struct timespec startExecTs;
		struct timespec endExecTs;

		struct timespec startWtTs;
		struct timespec endWtTs;

		/**
		 * The following gets the wall time, for determining next execution time.
		 */
//		std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
		clock_gettime(CLOCK_REALTIME, &startWtTs);

		/**
		 * Obtain the cpu time at the start of this periodic task. This is for CPU time measurement.
		 **/
		clock_gettime(CLOCK_THREAD_CPUTIME_ID , &startExecTs);

		/**Now run the task.
		 * Call the task method.
		 */
		this->taskMethod();

		/**
		 *Now get the end CPU time entry.
		 **/
		clock_gettime(CLOCK_THREAD_CPUTIME_ID, &endExecTs);
		clock_gettime(CLOCK_REALTIME, &endWtTs);

		long deltaInus = DELTA_TIMESTAMPS(endExecTs, startExecTs);
		long executionTime = DELTA_TIMESTAMPS(endWtTs, startWtTs);

		/**
		 * Determine where we are in terms of the worst case execution time.
		 */
		if (deltaInus > worstCaseExecutionTime) {
			worstCaseExecutionTime = deltaInus;
		}
		lastExecutionTime = deltaInus;

		lastWallTime = executionTime;
		if (lastWallTime > worstCaseWallTime) {
			worstCaseWallTime = lastWallTime;
		}

		if (executionTime > taskPeriod) {
			missedDeadlineCount++;
		}

		/**
		 * Now figure out exactly what time it is to schedule the next execution.
		 */
		clock_gettime(CLOCK_REALTIME, &endWtTs);
		long deltaTime = DELTA_TIMESTAMPS(endWtTs, startWtTs);

		/**
		 * Figure out how long to sleep.
		 */
//		std::chrono::microseconds remainingSleepTime = std::chrono::microseconds(taskPeriod) - executionTime;
		long sleepTime = taskPeriod - deltaTime;

		struct timespec sleepDelay = {0,sleepTime*1000};
		if (sleepTime > 1000000)
		{
			sleepDelay.tv_sec = sleepTime / 1000000;
			sleepDelay.tv_nsec = sleepTime %1000000 * 1000;
		}


		/**
		 * Sleep until the next execution should occur.
		 */
		waitForNextExecution(sleepDelay);
	}
}
/**
 * This method will start up any runnable objects which are contained within a class that implements the RUnnable interface.
 * If there are no other objects that are runnable, there is no need to override this method.  However, if a child class
 * contains an instance of a runnable object, then this method must be overridden in the derived class.
 */
void PeriodicTask::startChildTasks() {
	// The default implementation does nothing.
}

/**
 * This method will stop any runnable objects which are contained within a class that implements the RUnnable interface.
 * If there are no other objects that are runnable, there is no need to override this method.  However, if a child class
 * contains an instance of a runnable object, then this method must be overridden in the derived class.
 */
void PeriodicTask::stopChildTasks() {
	// The default implementation does nothing.
}

/**
 * This method will start up any runnable objects which are contained within a class that implements the RUnnable interface.
 * If there are no other objects that are runnable, there is no need to override this method.  However, if a child class
 * contains an instance of a runnable object, then this method must be overridden in the derived class.
 *
 */
void PeriodicTask:: startChildRunnables()
{
	startChildTasks();
}

void PeriodicTask:: stopChildRunnables()
{
	stopChildTasks();
}


