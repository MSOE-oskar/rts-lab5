/**
 * @file LightController.cpp
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
 *      This class is a controller for the light.  It is responsible for managing the operation of the lights based upon incoming commands
 *      from the command queue.
 */

#include "CommandQueue.h"
#include "LightController.h"
#include "NetworkCommands.h"
#include "gpio/provided/GPIO.h"
#include "networking/provided/NetworkCommands.h"
#include "networking/provided/NetworkCommandTypes.h"
#include "queuing/CommandQueue.h"
#include "tasking/provided/PeriodicTask.h"
#include <chrono>
#include <thread>

LightController::LightController(int gpioOutPin, int gpioInPin, SWE4211RPi::CommandQueue &queue, std::string threadName, uint32_t period) : SWE4211RPi::PeriodicTask(threadName, period), referencequeue(queue), light(SWE4211RPi::GPIO::getInstance(gpioOutPin, SWE4211RPi::GPIO::GPIO_OUT, SWE4211RPi::GPIO::GPIO_HIGH)), pushbutton(SWE4211RPi::GPIO::getInstance(gpioInPin, SWE4211RPi::GPIO::GPIO_IN)) {}

LightController::~LightController()
{
    SWE4211RPi::GPIO::freeInstance(light);
    SWE4211RPi::GPIO::freeInstance(pushbutton);
}

void LightController::taskMethod()
{
    while (referencequeue.hasItem())
    {
        SWE4211RPi::CommandQueueEntry val = referencequeue.dequeue();

        if (val.commandType != COMMAND_MSG_TYPE) {
            continue;
        }

        if ((val.command & LIGHTOFFCMD) != 0) {
            lampOn = false;
        }

        if ((val.command & LIGHTONCMD) != 0) {
            lampOn = true;
        }

        if ((val.command & LIGHTPWMADJUSTMENTCMD) != 0 && val.parameter1 <= 1000) {
            dutyCycle = static_cast<int>(val.parameter1 / 10);
        }
    }

    if (!(lampOn || pushbutton.getValue() == SWE4211RPi::GPIO::GPIO_LOW))
    {
        light.setValue(SWE4211RPi::GPIO::GPIO_HIGH);
        return;
    }

    const uint64_t onTimeMicroseconds = static_cast<uint64_t>(getTaskPeriod()) * static_cast<uint64_t>(dutyCycle) / 100;
    light.setValue(SWE4211RPi::GPIO::GPIO_LOW);
    std::this_thread::sleep_for(std::chrono::microseconds(onTimeMicroseconds));
    light.setValue(SWE4211RPi::GPIO::GPIO_HIGH);
}