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

class ValueTx: public msg::Message {
private:
	uint32_t timestamp;		// 发送时间戳
	float* value;			// 数值数组
	uint8_t len;			// 数组长度
public:
	const uint8_t msg_id = EXPLOT_MSGID_VALUETX;

	ValueTx(uint8_t len);
	virtual ~ValueTx();
	virtual void Encode(uint8_t* payload);
	virtual void Decode(uint8_t* payload);
	virtual void UpdataData();
	virtual void Execute();
};


} /* namespace debugger */

#endif /* EXPLOTMSGS_H_ */
