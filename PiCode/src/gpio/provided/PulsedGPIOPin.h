/*
 * PulsedGPIOPin.h
 *
 *  Created on: Jun 7, 2023
 *      Author: se3910
 */

#ifndef PULSEDGPIOPIN_H_
#define PULSEDGPIOPIN_H_

#include <string>
#include "PeriodicTask.h"
#include "GPIO.h"

namespace SWE4211RPi {

class PulsedGPIOPin: public PeriodicTask {

public:
	/**
	 * This enumeration sets up the pin to either be an input or an output pin.
	 */
	enum PULSE_TYPE {
		ONE_SHOT, CONTINUOUS
	};

	/**
	 * This will setup a GPIO pin to be pulsed.
	 * @param number This is the GPIO pin that is to be pulsed.
	 * @param threadName This si the name of the thread for the pin.
	 * @param period This is the period of the task that is to cause a pulse.
	 * @param type This is the type of the pulse, either One Shot, which fires once, or COntinuous, which will have a given pulse every period units of time.
	 * @param dir This si the direction for the pin - out for it to be meaningful.
	 * @param val This is the default pin state value, which tends to be low.
	 */
	PulsedGPIOPin(int number, std::string threadName, uint32_t period, PULSE_TYPE type = CONTINUOUS, GPIO::DIRECTION dir= GPIO::GPIO_OUT, GPIO::VALUE val = GPIO::GPIO_LOW);

	virtual ~PulsedGPIOPin();

	/**
	 * This method will drive an output pin to the appropriate value.
	 * @param This is the value that is to be written to the pin.
	 */
	virtual void setValue(GPIO::VALUE);

	/**
	 * This method will set the pulse duration, or nhow long the pulse should be on.
	 * @param duration This is the time on.
	 */
	virtual void setpulseDuration(int duration);

	/**
	 * This is the overridden taskMethod that is periodically invoked.
	 */

	virtual void taskMethod();
	/**
	 * this method will stop the thread and its execution.  In the case of this particular class, it will also shutdown the network socket.
	 */
	void stopThreadExecution();

private:
	GPIO & gpioPinInstance;

	/**
	 * The duration of each pulse.
	 */
	int pulseDuration = 0;
	/**
	 * The initial value for the GPIO pin.
	 */
	SWE4211RPi::GPIO::VALUE initialValue = SWE4211RPi::GPIO::GPIO_LOW;

	/**
	 * This defines the type of the pulse.
	 */
	PulsedGPIOPin::PULSE_TYPE type = PULSE_TYPE::CONTINUOUS;


	bool firePulse = false;

	/**
	 * This method will cause a sleep to occur and then delay after that period of time.
	 * @param specificDelay
	 * @param value
	 */
	void delayedToggleAndSleep(long specificDelay, SWE4211RPi::GPIO::VALUE value);
};
}
#endif /* PULSEDGPIOPIN_H_ */
