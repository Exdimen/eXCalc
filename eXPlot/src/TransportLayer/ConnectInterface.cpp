/*
 * ConnectInterface.cpp
 *
 *  Created on: 2022年11月4日
 *      Author: Exdimen
 */

#include <chrono>
#include <thread>

#include "ConnectInterface.h"





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


    hepc->InitRx(ComRead);

    // // 配置循环发送句柄
    // htx->SetUartConnect(hepc);
    // htx->AddMsg(msg_vt, "value plot", 1, true);
}

void loop() {
//     #ifdef SBUS_TEMP
//     hst->Update();
//     #endif
    hepc->Update();
    serial_interface->UpdateRxSpeed();
//     htx->Update();
// //    hsbus->Update();
}
