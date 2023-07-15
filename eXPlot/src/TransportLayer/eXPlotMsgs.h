/*
 * eXPlotMsgs.h
 *
 *  Created on: Nov 22, 2022
 *      Author: Exdimen
 */

#ifndef EXPLOTMSGS_H_
#define EXPLOTMSGS_H_

#include <Message.h>
#include "eXPlotConnect.h"
namespace debugger {
class ValueTx;
typedef void (* SendCallback)(ValueTx*);
class ValueTx: public msg::Message {
private:
	uint32_t timestamp;		// 发送时间戳

	SendCallback send_call;	// 发送时的回调函数，用于填充数据
public:
	const uint8_t msg_id = EXPLOT_MSGID_VALUETX;
	float* value;			// 数值数组
	uint8_t len;			// 数组长度
	ValueTx(uint8_t len);
	virtual ~ValueTx();
	virtual void Encode(uint8_t* payload);
	virtual void Decode(uint8_t* payload);
	virtual void UpdataData();
	virtual void Execute();
	virtual uint8_t GetMsgID() {return msg_id;}
	void SetSendCallback(SendCallback _send_call) {send_call = _send_call;}
};


} /* namespace debugger */

#endif /* EXPLOTMSGS_H_ */
