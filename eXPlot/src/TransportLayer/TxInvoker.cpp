/*
 * TxInvoker.cpp
 *
 *  Created on: 2022年11月24日
 *      Author: Exdimen
 */

#include <TxInvoker.h>
#include "SystemExtra.h"
namespace msg {


TxInvoker::~TxInvoker() {
	// TODO Auto-generated destructor stub
}

void TxInvoker::AddMsg(Message* add_msg, std::string label, unsigned int period, bool is_enable) {
	list[msg_count].label = label;
	list[msg_count].msg = add_msg;
	list[msg_count].period = period;
	list[msg_count].last_timestamp = millis();
	list[msg_count].is_enable = is_enable;
	msg_count ++;
}
void TxInvoker::Update() {
	if (uart_cont != nullptr) {
		for (uint8_t i=0; i<msg_count; i++) {
			if (list[i].is_enable && TimeUpCheck(list[i].last_timestamp + list[i].period)) {
				list[i].msg->UpdataData();
				uart_cont->LaunchSend(list[i].msg);
				list[i].last_timestamp = millis();
			}
		}
	}
}
} /* namespace msg */
