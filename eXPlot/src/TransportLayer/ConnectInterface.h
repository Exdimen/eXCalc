/*
 * ConnectInterface.h
 *
 *  Created on: 2022年11月4日
 *      Author: Exdimen
 */

#ifndef CONNECTINTERFACE_H_
#define CONNECTINTERFACE_H_


#include <CSerialPort/SerialPort.h>
#include <CSerialPort/SerialPortInfo.h>


#include "eXPlotConnect.h"
#include "eXPlotMsgs.h"

#include "SerialInterface.h"

// 传输链接句柄



#define CHANNEL_THRESHOLD   0.2f        // 通道阈值 

// using namespace sbus;


extern debugger::eXPlotConnect* hepc;
extern itas109::CSerialPort* serial_port;
extern edm_comm::SerialInterface* serial_interface;

// extern CSerialPort* Serial;


void Connect();

#endif /* CONNECTINTERFACE_H_ */
