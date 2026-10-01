/**
 * @file NetworkManager.cpp
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
 * This file defines the implementation for the Network Manager.  The Network Manager manages network connections and acts as a server, receiving messages sent over a socket.
 */

#include "NetworkManager.h"
#include "NetworkCfg.h"
#include "CommandQueue.h"
#include "NetworkMessage.h"
#include "NetworkCommands.h"
#include "NetworkDebugLibrary.h"
#include <errno.h>  // errno, EINTR
#include <stdio.h>  // perror, fprintf
#include <string.h> // memset
#include <unistd.h>
#include <sys/types.h> // ssize_t
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> // htons, ntohl
#include <poll.h>
#include <string>

namespace SWE4211RPi
{

    NetworkManager::NetworkManager(unsigned short port, CommandQueue *queues[], std::string threadName)
        : RunnableClass(threadName),
          portNumber(port),
          referencequeues(queues)
    {
    }

    NetworkManager::~NetworkManager()
    {
        // main owns the queue array, so it is not deleted here.
        if (connectedSocket >= 0)
        {
            close(connectedSocket);
        }
        if (server_fd >= 0)
        {
            close(server_fd);
        }
    }

    void NetworkManager::run()
    {
        int opt = 1;
        struct sockaddr_in address;
        memset(&address, 0, sizeof(address));

        server_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (server_fd < 0)
        {
            perror("Failed to create socket");
            return;
        }

        setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

        address.sin_family = AF_INET;
        address.sin_addr.s_addr = htonl(INADDR_ANY);
        address.sin_port = htons(portNumber);

        if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
        {
            perror("Bind failed");
            return;
        }

        if (listen(server_fd, 1) < 0)
        {
            perror("Listen failed");
            return;
        }

        printf("Server listening on port %d...\n", portNumber);

        while (keepGoing)
        {
            // Short timeout so a stop request is noticed.
            struct pollfd listenPoll = {server_fd, POLLIN, 0};
            if (poll(&listenPoll, 1, 100) <= 0)
            {
                continue;
            }

            struct sockaddr_in clientAddress;
            socklen_t clientLength = sizeof(clientAddress);
            connectedSocket = accept(server_fd, (struct sockaddr *)&clientAddress, &clientLength);
            if (connectedSocket < 0)
            {
                continue;
            }

            reportNewClientConnection(clientAddress);

            networkMessageStruct msg;
            while (keepGoing && receiveNetworkMessage(msg))
            {
                processReceivedMessage(msg);
            }

            close(connectedSocket);
            connectedSocket = -1;

            memset(&clientAddress, 0, sizeof(clientAddress));
            reportNewClientConnection(clientAddress);
        }
    }

    void NetworkManager::stopThreadExecution()
    {
        RunnableClass::stopThreadExecution();
    }

    int NetworkManager::getSocketID()
    {
        return connectedSocket;
    }

    bool NetworkManager::receiveNetworkMessage(networkMessageStruct &message)
    {
        // Fields are left in network byte order.
        uint8_t *buffer = (uint8_t *)&message;
        size_t total = 0;

        while (total < sizeof(message))
        {
            if (!keepGoing)
            {
                return false;
            }

            // Short timeout so a stop request is noticed.
            struct pollfd clientPoll = {connectedSocket, POLLIN, 0};
            int ready = poll(&clientPoll, 1, 100);
            if (ready == 0 || (ready < 0 && errno == EINTR))
            {
                continue;
            }
            if (ready < 0)
            {
                return false;
            }

            ssize_t count = recv(connectedSocket, buffer + total, sizeof(message) - total, 0);
            if (count < 0 && errno == EINTR)
            {
                continue;
            }
            if (count <= 0)
            {
                return false;
            }
            total += (size_t)count;
        }
        return true;
    }

    int NetworkManager::processReceivedMessage(networkMessageStruct &receivedMessage)
    {
        networkMessageStruct converted{
            ntohl(receivedMessage.securityMode),
            ntohl(receivedMessage.messageID),
            ntohl(receivedMessage.timestampHigh),
            ntohl(receivedMessage.timestampLow),
            ntohl(receivedMessage.messageType),
            ntohl(receivedMessage.messageDestination),
            ntohl(receivedMessage.message),
            ntohl(receivedMessage.parameter1),
            ntohl(receivedMessage.parameter2),
            ntohl(receivedMessage.xorChecksum)};

        uint32_t convertedXorChecksum =
            converted.securityMode ^
            converted.messageID ^
            converted.timestampHigh ^
            converted.timestampLow ^
            converted.messageType ^
            converted.messageDestination ^
            converted.message ^
            converted.parameter1 ^
            converted.parameter2;

        if (convertedXorChecksum != converted.xorChecksum)
        {
            return convertedXorChecksum;
        }

        if (converted.messageType != COMMAND_MSG_TYPE)
        {
            return convertedXorChecksum;
        }

        // Destination 1 maps to queue 0, 2 to queue 1.
        if (converted.messageDestination < 1 || converted.messageDestination > NUMBER_OF_QUEUES)
        {
            return convertedXorChecksum;
        }

        CommandQueueEntry entry{converted.messageType, converted.message, converted.parameter1, converted.parameter2};

        referencequeues[converted.messageDestination - 1]->enqueue(entry);

        return convertedXorChecksum;
    }

    void NetworkManager::reportNewClientConnection(const struct sockaddr_in &clientAddress)
    {
        CommandQueueEntry entry{0, REPORT_NEW_IPADDRESS, ntohl(clientAddress.sin_addr.s_addr), 0};
        for (int i = 0; i < NUMBER_OF_QUEUES; i++)
            referencequeues[i]->enqueue(entry);
    }

} // namespace SWE4211RPi
