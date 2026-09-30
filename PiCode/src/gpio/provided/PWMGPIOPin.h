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
#ifndef PWMGPIOPIN_H_
#define PWMGPIOPIN_H_

#include "PulsedGPIOPin.h"

namespace SWE4211RPi {

class PWMGPIOPin : public PulsedGPIOPin {
public:
	/**
	 * This method will instantiate a new instance of this class.
	 * @param number This is the GPIO pin that is to be pulsed.
	 * @param threadName This is the name of the thread for this PIN.
	 * @param period This is the period for the PWM signal.
	 */
	PWMGPIOPin(int number, std::string threadName, uint32_t period);

	/**
	 * Destructor for the file.
	 */
	virtual ~PWMGPIOPin();

	/**
	 * This method will set the duty cycle.
	 * @param dc This is the duty cycle in .1% increments.  Must be between 0 and 1000 or no change will occur.
	 */
	void setDutyCycle(int dc);

private:
	/**
	 * This is the duty cycle for the given pin.  The value can range between 0 and 1000.
	 */
	int dutyCycle;
};
}
#endif /* PWMGPIOPIN_H_ */




