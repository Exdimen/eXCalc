#pragma once

#include <UartConnect.h>

namespace sensor {
#define EXPLOT_HEADER 			0xB0
#define SENSOR_MSGID_POSITION 	0x01
class VisionSensorConnect: public UartConnect {
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
	VisionSensorConnect(ConnectType cont_type);
	virtual ~VisionSensorConnect();
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
