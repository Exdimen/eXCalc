/*
 * ConnectInterface.cpp
 *
 *  Created on: 2022年11月4日
 *      Author: Exdimen
 */

#include <chrono>
#include <thread>

#include "ConnectInterface.h"


// 循环发送句柄
msg::TxInvoker* htx = new msg::TxInvoker();
// 循环发送数据包
debugger::ValueTx* msgtx_vt = new debugger::ValueTx(1);


sensor::VisionSensorConnect* hvsc = new sensor::VisionSensorConnect(UartConnect::RX);
debugger::eXPlotConnect* hepc = new debugger::eXPlotConnect(UartConnect::RX_AND_TX);
itas109::CSerialPort* serial_port = new itas109::CSerialPort();

edm_comm::SerialInterface* serial_interface = new edm_comm::SerialInterface(serial_port);

uint8_t read_data;
uint8_t send_data;

void ValueTxSendCallback(debugger::ValueTx* msg);




void setup();
void loop();

bool ComRead(uint8_t* buffer) {
    if (serial_interface) {
        return serial_interface->SerialRecevice(buffer);
    }
    return false;
}
bool ComWrite(uint8_t* buffer, uint16_t len) {
    if (serial_interface) {
        return serial_interface->SerialTransmit(buffer, len);
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


    hepc->Init(ComRead);
    hepc->Init(ComWrite);
    // hvsc->Init(ComRead);
    
    msgtx_vt->SetSendCallback(ValueTxSendCallback);
    // 配置循环发送句柄
    htx->SetUartConnect(hepc);
    htx->AddMsg(msgtx_vt, "value plot", 5, true);
}

void loop() {
//     #ifdef SBUS_TEMP
//     hst->Update();
//     #endif
    // 接收通信句柄 
    hvsc->Update();
    hepc->Update();
    // 定期发送句柄
    htx->Update();
    // 计算通信速度
    serial_interface->UpdateRxSpeed();

// //    hsbus->Update();
}

void ValueTxSendCallback(debugger::ValueTx* msg) {
    msg->value[0] = 1;
}
