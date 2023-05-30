/*
 * eXPlotConnect.h
 *
 *  Created on: Nov 22, 2022
 *      Author: Exdimen
 */

#ifndef EXPLOTCONNECT_H_
#define EXPLOTCONNECT_H_

#include <UartConnect.h>

namespace debugger {
#define EXPLOT_HEADER 			0xB0
#define EXPLOT_MSGID_VALUETX 	0x01
class eXPlotConnect: public UartConnect {
private:
	const uint8_t header = 0xB0;
protected:


	virtual bool Check();
	virtual uint8_t GenerateCheck();
	virtual bool Parse();
	virtual void Decode();
	virtual void Encode(){};
	virtual void Send();
	virtual void TxUpdate() {return;}

public:
	eXPlotConnect(ConnectType cont_type);
	virtual ~eXPlotConnect();
	/**
	 * @brief 参考MotorSendCallback()，发送的填充回调函数
	 * 
	 * @param value 发送float变量数组
	 * @param len 数组长度
	 * @return true 
	 * @return false 
	 */
	bool LaunchSend(msg::Message* msg);

};


} /* namespace debugger */

#endif /* EXPLOTCONNECT_H_ */
