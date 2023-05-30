/*
 * ConnectInterface.cpp
 *
 *  Created on: 2022年11月4日
 *      Author: Exdimen
 */


#include "ConnectInterface.h"




// 传输链接句柄
debugger::eXPlotConnect* hepc = new debugger::eXPlotConnect(UartConnect::RX);
CSerialPort* Serial = new CSerialPort();


uint8_t read_data;
uint8_t send_data;

void setup();
void loop();

bool ComRead(uint8_t* buffer) {
    if (Serial->m_bOpen == true) {
        return Serial->readFromComm(buffer);
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
    Serial->UpdateRxSpeed();
//     htx->Update();
// //    hsbus->Update();
}
