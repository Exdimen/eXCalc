/*
 * Message.h
 *
 *  Created on: Nov 22, 2022
 *      Author: Exdimen
 */

#ifndef MESSAGE_H_
#define MESSAGE_H_

#include "stdint.h"
#include "string.h"

namespace msg {

class Message {
protected:
	uint8_t len_cnt;
public:
	const uint8_t msg_id = 0x00;		// 命令码
	const uint8_t address = 0x00;		// 地址
	Message();
	virtual ~Message();
	virtual void Decode (uint8_t* payload) {};
	virtual void Encode(uint8_t* payload) {};
	virtual void Execute() {};
	virtual void UpdataData() {};		// 循环发送时更新数据
	uint8_t GetLen() {return len_cnt;} // 获取载荷长度，编码后有效
};

} /* namespace msg */

#endif /* MESSAG.E_H_ */
