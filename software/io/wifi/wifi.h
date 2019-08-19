/*
 * wifi.h
 *
 *  Created on: Sep 2, 2018
 *      Author: gideon
 */

#ifndef WIFI_H_
#define WIFI_H_

#include "fastuart.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

// This class provides an interface to the WiFi module
// Initially, a simple Uart based interface that links to a TCP socket
// Later, this class can provide the bridge between the 6551 emulation and the WiFi module
// or even use the SPI interface for faster transfers

class WiFi
{
    FastUART *uart;
    SemaphoreHandle_t rxSemaphore;
    TaskHandle_t relayTask;
    QueueHandle_t commandQueue;

    bool doClose;

    void Enable();
    void Disable();
    void Boot();
    int  Download(const uint8_t *binary, uint32_t address, uint32_t length);
    void PackParams(uint8_t *buffer, int numparams, ...);
    bool Command(uint8_t opcode, uint16_t length, uint8_t chk, uint8_t *data, uint8_t *receiveBuffer, int timeout);

    static void TaskStart(void *context);
    static void EthernetRelay(void *context);
    void Thread();
    void Listen();
    int actualSocket;
public:
    WiFi();
    void Quit();

    BaseType_t doBootMode();
    BaseType_t doStart();
    BaseType_t doDownload(uint8_t *binary, uint32_t address, uint32_t length, bool doFree);
};


extern WiFi wifi;


#endif /* WIFI_H_ */
