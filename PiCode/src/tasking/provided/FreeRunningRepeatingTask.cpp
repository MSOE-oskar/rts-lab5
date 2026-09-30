/*
 * FreeRunningRepeatingTask.cpp
 *
 *  Created on: Jun 10, 2023
 *      Author: se3910
 */

#include "FreeRunningRepeatingTask.h"
#include <iostream>
#include <chrono>
#include <iomanip>

#define DELTA_TIMESTAMPS(endTS, startTS) ((endTS.tv_sec * 1000000 + endTS.tv_nsec / 1000) - (startTS.tv_sec * 1000000 + startTS.tv_nsec / 1000))

using namespace SWE4211RPi;

/**
 * This is the default constructor for the class.
 * @param threadName This is the name of the thread in a human readable format.
 * @param period This is the period for the task, given in microseconds.
 * Must be at least 100 microseconds.
 */
FreeRunningRepeatingTask::FreeRunningRepeatingTask(std::string threadName,
		int priority) :
		RunnableClass(threadName, priority) {
	this->setPriority(priority);
}

/**
 * This method will clean up from the periodic task and its execution.
 */
FreeRunningRepeatingTask::~FreeRunningRepeatingTask() {
	/**
	 * Nothing to be done in the destructor.
	 */

}

/**
 * This method will obtain the CPU usage.  CPU usage is the worst case measured execution time divided by the period.
 * @return The CPU usage will be returned.
 */
double FreeRunningRepeatingTask::getCPUUsageInfo() {
	double cpuUsage = ((double) worstCaseExecutionTime
			/ (double) worstCaseWallTime) * 100.0;
	return cpuUsage;
}

/**
 * This method will print out information about the given thread.  The info will be dependent upon the given thread.
 */
void FreeRunningRepeatingTask::printInformation() {
	std::cout << myOSThreadID << "\t" << std::setw(23) << myName
			<< std::setw(5) << getPriority() << "\tFRT\t " << std::setw(10) << 0
			<< "\t " << std::setw(12) << lastExecutionTime << "\t "
			<< std::setw(8) << worstCaseExecutionTime << "\t " << std::setw(12)
			<< lastWallTime << "\t " << std::setw(8) << worstCaseWallTime
			<< "\t" << std::fixed << std::setprecision(3) << getCPUUsageInfo()
			<< "%";
	if (missedDeadlineCount > 0) {
		std::cout << missedDeadlineCount;
	}
	std::cout << "\n";
}

/**
 * This method will reset thread diagnostics back to their default values.
 */
void FreeRunningRepeatingTask::resetThreadDiagnostics() {
	// Reset all diagnostic variables to zero.
	worstCaseExecutionTime = 0;
	lastExecutionTime = 0;
	lastWallTime = 0;
	worstCaseWallTime = 0;
}

/**
 * This is a private method that will be used by start to invoke the run method.
 */
void FreeRunningRepeatingTask::run() {
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
		clock_gettime(CLOCK_REALTIME, &startWtTs);

		/**
		 * Obtain the cpu time at the start of this periodic task. This is for CPU time measurement.
		 **/
		clock_gettime(CLOCK_THREAD_CPUTIME_ID, &startExecTs);

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

		// Allow other tasks to run here.
		std::this_thread::yield();
	}
}
/**
 * This method will start up any runnable objects which are contained within a class that implements the RUnnable interface.
 * If there are no other objects that are runnable, there is no need to override this method.  However, if a child class
 * contains an instance of a runnable object, then this method must be overridden in the derived class.
 *
 */
void FreeRunningRepeatingTask::startChildTasks() {
	// The default implementation does nothing.
}

void FreeRunningRepeatingTask::stopChildTasks() {
	// The default implementation does nothing.
}

/**
 * This method will start up any runnable objects which are contained within a class that implements the RUnnable interface.
 * If there are no other objects that are runnable, there is no need to override this method.  However, if a child class
 * contains an instance of a runnable object, then this method must be overridden in the derived class.
 *
 */
void FreeRunningRepeatingTask::startChildRunnables() {
	startChildTasks();
}

void FreeRunningRepeatingTask::stopChildRunnables() {
	stopChildTasks();
}

