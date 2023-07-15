/*
 * ConnectInterface.cpp
 *
 *  Created on: 2022年11月4日
 *      Author: Exdimen
 */

#include <chrono>
#include <thread>

#include "ConnectInterface.h"


sensor::VisionSensorConnect* hvsc = new sensor::VisionSensorConnect(UartConnect::RX);
debugger::eXPlotConnect* hepc = new debugger::eXPlotConnect(UartConnect::RX);
itas109::CSerialPort* serial_port = new itas109::CSerialPort();

edm_comm::SerialInterface* serial_interface = new edm_comm::SerialInterface(serial_port);

uint8_t read_data;
uint8_t send_data;

void setup();
void loop();

bool ComRead(uint8_t* buffer) {
    if (serial_interface) {
        return serial_interface->SerialRecevice(buffer);
    }
    return false;
}
void Connect()
{
    /* USER CODE BEGIN ConnectTask */
    setup();
    /* Infinite loop */
    for(;;)
    {
        loop();
        std::this_thread::sleep_for(std::chrono::microseconds(500));
    }
    /* USER CODE END ConnectTask */
}

void setup() {


    // hepc->InitRx(ComRead);
    hvsc->InitRx(ComRead);
    // // 配置循环发送句柄
    // htx->SetUartConnect(hepc);
    // htx->AddMsg(msg_vt, "value plot", 1, true);
}

void loop() {
//     #ifdef SBUS_TEMP
//     hst->Update();
//     #endif
    hvsc->Update();
    hepc->Update();
    serial_interface->UpdateRxSpeed();
//     htx->Update();
// //    hsbus->Update();
}
