/*
 * ConnectInterface.h
 *
 *  Created on: 2022年11月4日
 *      Author: Exdimen
 */

#ifndef CONNECTINTERFACE_H_
#define CONNECTINTERFACE_H_


#include "eXPlotConnect.h"
#include "eXPlotMsgs.h"
#include "CSerialPort/SerialPort.h"
#include "CSerialPort/SerialPortInfo.h"
#include "SerialInterface.h"

// 传输链接句柄
debugger::eXPlotConnect* hepc = new debugger::eXPlotConnect(UartConnect::RX);
itas109::CSerialPort* serial_port = new itas109::CSerialPort();

edm_comm::SerialInterface* serial_interface = new edm_comm::SerialInterface(serial_port);


#define CHANNEL_THRESHOLD   0.2f        // 通道阈值 

// using namespace sbus;

// extern CSerialPort* Serial;


void Connect();

#endif /* CONNECTINTERFACE_H_ */
