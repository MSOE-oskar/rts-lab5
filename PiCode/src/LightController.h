/**
 * @file LightController.h
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
 *
 * This class is a controller for lights.  It is responsible for turning lights on or off based on transmitted messages.
 */
#ifndef LIGHTCONTROLLER_H
#define LIGHTCONTROLLER_H

#include <pthread.h>
#include "GPIO.h"
#include "CommandQueue.h"
#include "PeriodicTask.h"

class LightController: public SWE4211RPi::PeriodicTask {
private:
	/**
	 * This is the queue that is going to be read for the purpose of controlling the light.
	 */
	SWE4211RPi::CommandQueue& referencequeue;
	/**
	 * This is the light that is controlled by the class.
	 */
	SWE4211RPi::GPIO& light;

	/**
	 * This is the GPIO instance representing the input pushbutton.
	 */
	SWE4211RPi::GPIO& pushbutton;

	/**
	 * This value will keep track of the duty cycle for the PWM of this device.  The duty cycle is what percentage of time the light is on.
	 * It can range between 0 (inclusive) and 100 (inclusive)
	 */
	int dutyCycle=50;

	/**
	 * This value will keep track of the state of the lamp.  If the value is false, the lamp is off.  If the value is true, the lamp should be turned on at the given brightness based upon the duty cycle.
	 */
	bool lampOn = false;

public:
	/**
	 * This constructor will instantiate a new instance of the light controller.
	 * @param gpioOutPin This is the GPIO pin that is to be controlled by this device.
	 * @param gpioInPin This is the GPIO pin that will impact light control.
	 * @param queue This is the instance of the queue that will be used to send commands to the light controller.
	 * @param threadName This is the name given to the executing thread.  It is a simple, string that is human readable.
	 * @param period This is the period for the task, given in microseconds. Must be at least 100 microseconds.
	 */
	LightController(int gpioOutPin, int gpioInPin, SWE4211RPi::CommandQueue& queue, std::string threadName, uint32_t period);

	/**
	 * This destructor destroys the existing instance of the light controller.
	 */
	virtual ~LightController();

	/**
	 * This is the task method.  The task method will be invoked periodically every period
	 * units of time.
	 */
	void taskMethod();
};

#endif /* LIGHTCONTROLLER_H */
