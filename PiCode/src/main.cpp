/*
 * main.cpp

 *
 *  Created on: Mar 3, 2015
 *      Author: student
 */
#include <iostream>
#include "LightController.h"
#include "NetworkManager.h"
#include <ifaddrs.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace std;
using namespace SWE4211RPi;

void printIPs();

/**
 * This is the main program.  It will instantiate an instance of a Queue, a Network Manager, and a Light Controller.  Start will then be called on each of those classes
 * and the priorities will be set.
 */
int main(int argc, char* argv[]) {
	CommandQueue *myQueue[2];
	myQueue[0] = new CommandQueue();
	myQueue[1] = new CommandQueue();

	/**
	 * Instantiate a network manager and a pair of light controllers.  The light controller should execute every 50 ms.
	 */
	NetworkManager nm(9090, myQueue, "Network Thread");
	LightController lc1(13, 16, *myQueue[0], "Light Controller Thread 1", 10000);
	LightController lc2(21, 19, *myQueue[1], "Light Controller Thread 2", 10000);

	// Print the IP address of the device out to the console.
	printIPs();

	// Start each of the two threads up.
	nm.start();
	lc1.start();
	lc2.start();

	// Wait for the user to press a key.
	char msg[1024];
	cin >> msg;
	cout << msg;

	// Shutdown the network managers.
	nm.stopThreadExecution();
	lc1.stopThreadExecution();
	lc2.stopThreadExecution();
	
	// Enqueue two zero messages in a row to the two queues.
	CommandQueueEntry nothing = {0,0, 0, 0};

	myQueue[0]->enqueue(nothing);
	myQueue[1]->enqueue(nothing);
	
	// Wait for the threads to terminate.
	lc1.waitForShutdown();
	lc2.waitForShutdown();
	nm.waitForShutdown();
	
	// Free the queues.
	delete myQueue[0];
	delete myQueue[1];
}

// Note: Code modified from : https://stackoverflow.com/questions/212528/how-can-i-get-the-ip-address-of-a-linux-machine
// Not the greatest code, but serves a purpose.
void printIPs()
{
    struct ifaddrs * ifAddrStruct=NULL;
    struct ifaddrs * ifa=NULL;
    void * tmpAddrPtr=NULL;

    getifaddrs(&ifAddrStruct);

    for (ifa = ifAddrStruct; ifa != NULL; ifa = ifa->ifa_next) {
        if (!ifa->ifa_addr) {
            continue;
        }
        if (ifa->ifa_addr->sa_family == AF_INET) { // check it is IP4 is a valid IP4 Address
            tmpAddrPtr=&((struct sockaddr_in *)ifa->ifa_addr)->sin_addr;
            char addressBuffer[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
            printf("%s IP Address %s\n", ifa->ifa_name, addressBuffer);
        } else if (ifa->ifa_addr->sa_family == AF_INET6) { // check it is IP6 is a valid IP6 Address
            tmpAddrPtr=&((struct sockaddr_in6 *)ifa->ifa_addr)->sin6_addr;
            char addressBuffer[INET6_ADDRSTRLEN];
            inet_ntop(AF_INET6, tmpAddrPtr, addressBuffer, INET6_ADDRSTRLEN);
            printf("%s IP Address %s\n", ifa->ifa_name, addressBuffer);
        }
    }
    if (ifAddrStruct!=NULL) freeifaddrs(ifAddrStruct);
}
