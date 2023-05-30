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
#include "SerialPort.h"


#define CHANNEL_THRESHOLD   0.2f        // 通道阈值 

// using namespace sbus;

extern CSerialPort* Serial;


void Connect();

#endif /* CONNECTINTERFACE_H_ */
