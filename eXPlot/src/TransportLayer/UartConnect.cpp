/*
 * Connect.cpp
 *
 *  Created on: 2022年11月4日
 *      Author: Exdimen
 */

#include <UartConnect.h>


UartConnect::UartConnect() {
	is_complete = false;
	is_initalized = false;
	read_data = 0;
	read_func = nullptr;
	state = HEADER;
	payload_cnt = 0;
	crc_cnt = 0;
	rx_msg = nullptr;
	tx_msg = nullptr;
	cont_type = RX;
	seq = 0;
	rx_cnt = 0;
	tx_cnt = 0;
}

UartConnect::~UartConnect() {

}



void UartConnect::RxUpdate() {
	if (read_func(&read_data) && is_initalized) {
		if (Parse()) {
			Decode();
			if (rx_msg != nullptr) {
				rx_msg->Execute();
				delete rx_msg;
			}
		}
	}
}

void UartConnect::Update() {
	if (is_initalized) {
		if (cont_type == RX || cont_type == RX_AND_TX) {
			RxUpdate();
		}
		if (cont_type == TX || cont_type == RX_AND_TX) {
			TxUpdate();
		}

	}
}

void UartConnect::Init(UartRecevice _read_func) {
	// 设置接收函数
    SetReadFunc(_read_func);
	
	is_initalized = true;
}
void UartConnect::Init(UartTransmit _transmit_func) {
	SetTransmitFunc(_transmit_func);

	is_initalized = true;
}