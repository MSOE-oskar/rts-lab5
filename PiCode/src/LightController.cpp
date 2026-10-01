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
#include "queuing/CommandQueue.h"
#include "tasking/provided/PeriodicTask.h"


LightController::LightController(int gpioOutPin, int gpioInPin, SWE4211RPi::CommandQueue& queue, std::string threadName, uint32_t period) : SWE4211RPi::PeriodicTask(threadName, period), referencequeue(queue),light(SWE4211RPi::GPIO::getInstance(gpioOutPin, SWE4211RPi::GPIO::GPIO_OUT)), pushbutton(SWE4211RPi::GPIO::getInstance(gpioInPin, SWE4211RPi::GPIO::GPIO_IN)) {}

LightController::~LightController() {
    delete &referencequeue;
    delete &light;
    delete &pushbutton;
}

void LightController::taskMethod() {
    int percent;
    while (referencequeue.hasItem()) {
        SWE4211RPi::CommandQueueEntry val = referencequeue.dequeue();

        switch (val.command) {
            case LIGHTOFFCMD:
                light.setValue(SWE4211RPi::GPIO::GPIO_HIGH);
                break;
            case LIGHTONCMD:
                light.setValue(SWE4211RPi::GPIO::GPIO_LOW);
                break;
            case LIGHTPWMADJUSTMENTCMD:
                if (val.parameter1.length() >= 0 || val.parameter2.length() <= 1000) {
                    percent = val.parameter1 / 10;
                }
                break;
            default:
                std::cerr << "IN SWITCH" << std::endl;
                break;
        }
    }


}