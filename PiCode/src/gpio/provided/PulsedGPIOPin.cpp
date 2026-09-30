/*
 * PulsedGPIOPin.cpp
 *
 *  Created on: Jun 7, 2023
 *      Author: se3910
 */

#include "PulsedGPIOPin.h"
#include "GPIO.h"

using namespace SWE4211RPi;

PulsedGPIOPin::PulsedGPIOPin(int number, std::string threadName,
		uint32_t period, PULSE_TYPE type, GPIO::DIRECTION dir, GPIO::VALUE val) :
		PeriodicTask(threadName, period) ,
		gpioPinInstance(SWE4211RPi::GPIO::getInstance(number, dir, val))
		{
	initialValue = val;
	this->type = type;
}

PulsedGPIOPin::~PulsedGPIOPin() {
	//this->setValue(initialValue);
}

void PulsedGPIOPin::setpulseDuration(int duration) {
	pulseDuration = duration;
	firePulse = true;
}

void PulsedGPIOPin::setValue(GPIO::VALUE value)
{

}

void PulsedGPIOPin::taskMethod() {

	if (firePulse || type == PULSE_TYPE::CONTINUOUS) {

		 long stime = pulseDuration;

		if (initialValue == SWE4211RPi::GPIO::GPIO_LOW)
			// Start by turning on the pin, which is the opposite of the initial value.
			gpioPinInstance.setValue(SWE4211RPi::GPIO::GPIO_HIGH);
		else {
			gpioPinInstance.setValue(SWE4211RPi::GPIO::GPIO_LOW);
		}

		// Go to sleep and then return to the initial value.
		//delayedToggleAndSleep(stime, initialValue);

		struct timespec sleepDelay = {0,stime*1000};
		nanosleep(&sleepDelay, NULL);

		gpioPinInstance.setValue(initialValue);

		// Return to the initial value.
		firePulse = false;
	}
}

void PulsedGPIOPin::delayedToggleAndSleep(long specificDelay, SWE4211RPi::GPIO::VALUE value)
{
	    struct sched_param param;
	    param.sched_priority = 55;
	    sched_setscheduler(0, RT_SCHEDULER_TO_USE, &param);

		struct timespec sleepDelay = {0,specificDelay*1000};
		nanosleep(&sleepDelay, NULL);

		gpioPinInstance.setValue(value);

	    param.sched_priority = getPriority();
	    sched_setscheduler(0, RT_SCHEDULER_TO_USE, &param);
}

void PulsedGPIOPin::stopThreadExecution() {
	gpioPinInstance.setValue(initialValue);
	PeriodicTask::stopThreadExecution();
	if (this->myThread!=NULL)
	{
		if (this->myThread->joinable())
		{
		this->myThread->detach();
		}
	}
}

