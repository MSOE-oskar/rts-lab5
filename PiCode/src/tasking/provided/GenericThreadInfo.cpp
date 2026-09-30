#include "GenericThreadInfo.h"
#include <sys/resource.h>
#include <sched.h>            /* Definition of SCHED_* constants */
#include <sys/syscall.h>      /* Definition of SYS_* constants */
#include <unistd.h>

using namespace SWE4211RPi;

// From https://sources.debian.org/src/util-linux/2.38.1-1.1/schedutils/sched_attr.h/
#ifndef SCHED_ATTR_SIZE_VER0
struct sched_attr {
	uint32_t size;
	uint32_t sched_policy;
	uint64_t sched_flags;

	/* SCHED_NORMAL, SCHED_BATCH */
	int32_t sched_nice;

	/* SCHED_FIFO, SCHED_RR */
	uint32_t sched_priority;

	/* SCHED_DEADLINE (nsec) */
	uint64_t sched_runtime;
	uint64_t sched_deadline;
	uint64_t sched_period;

	/* UTILIZATION CLAMPING */
	uint32_t sched_util_min;
	uint32_t sched_util_max;
};
#endif

/**
 * This is the default constructor for the class.
 * @param threadName This is the name of the thread in a human readable format.
 */
GenericThreadInfo::GenericThreadInfo(std::string threadName, pid_t myThreadID) :
		RunnableClass(threadName) {
	// Obtain the thread id by making a system call.
	myOSThreadID = myThreadID;
	int which = PRIO_PROCESS;
	this->priority = getpriority(which, myThreadID);

	struct sched_attr temp;

	syscall(SYS_sched_getattr, myThreadID, &temp, sizeof(struct sched_attr), 0);
	this->priority = temp.sched_priority;
}

/**
 * This method will clean up from the periodic task and its execution.
 */
GenericThreadInfo::~GenericThreadInfo() {
	// Nothing to do in the destructor.

}

/**
 * This is the run method for the class.
 */
void GenericThreadInfo::run() {
	// Do nothing, as this really is just for placeholder purposes.
}

