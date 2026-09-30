/**
 * @file PWMGPIOPin.h
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
 *      This file defines the behavior for a class which represents a pulsed PWM GPIO pin.
 */
#include "PWMGPIOPin.h"

using namespace SWE4211RPi;

/**
	 * This method will instantiate a new instance of this class.
	 * @param number This is the GPIO pin that is to be pulsed.
	 * @param threadName This is the name of the thread for this PIN.
	 * @param period This is the period for the PWM signal.
	 */
PWMGPIOPin::PWMGPIOPin(int number, std::string threadName, uint32_t period) : 	PulsedGPIOPin(number, threadName, period)
{
	this->dutyCycle = 0;
	this->setDutyCycle(0);
}

/**
 * Destructor for the file.
 */
PWMGPIOPin::~PWMGPIOPin() {
	// TODO Auto-generated destructor stub
}


void PWMGPIOPin::setDutyCycle(int dc)
{
	/**
	 * 1.0 If the duty cycle value is in range.
	 */
	if (dc >= 0 && dc <= 1000)
	{
		/**
		 * 1.1.0 Set the internal value.
		 */
		this->dutyCycle = dc;

		/**
		 * 1.2.0 Calculate the pulse duration as the duty cycle * taskPeriod.
		 */
		// Now calculate the pulse duration
		int pduration = (this->taskPeriod * dc)/1000;

		/**
		 * 1.3.0 Set the pulse duration.
		 */
		this->setpulseDuration(pduration);
	}
}


