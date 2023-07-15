/*
 * TxInvoker.h
 *
 *  Created on: 2022年11月24日
 *      Author: Exdimen
 */

#ifndef TXINVOKER_H_
#define TXINVOKER_H_
#include "Message.h"
#include "UartConnect.h"
#include "string"
namespace msg {

/**
 * @brief 串口循环发送功能
 * 
 */
class TxInvoker {
private:
	UartConnect* uart_cont;		// 发送连接
	struct MsgParam {
		std::string label;
		Message* msg;
		unsigned int period;
		unsigned int last_timestamp;
		bool is_enable;
	};
	MsgParam list[20];			// 循环发送的Msg列表
	uint8_t msg_count;			// Msg数
public:
	TxInvoker() : uart_cont(nullptr), msg_count(0) {};
	virtual ~TxInvoker();

	void SetUartConnect(UartConnect* _uart_cont) {uart_cont = _uart_cont;}
	void AddMsg(Message* add_msg, std::string label, unsigned int period, bool is_enable);

	void Update();
};

} /* namespace msg */

#endif /* TXINVOKER_H_ */
