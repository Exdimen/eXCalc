/*
 * eXPlotConnect.cpp
 *
 *  Created on: Nov 22, 2022
 *      Author: Exdimen
 */

#include "eXPlotConnect.h"
#include "string.h"
#include "eXPlotMsgs.h"
#include "CRCHub.h"
namespace debugger {


eXPlotConnect::eXPlotConnect(ConnectType _cont_type) {
	cont_type = _cont_type;
	crc_len = 2;
}


eXPlotConnect::~eXPlotConnect() {

}

bool eXPlotConnect::Parse() {
	switch (state) {
		case HEADER:	{	rx_cnt = 0;
							if (read_data == header) {
								rx_buffer[rx_cnt++] = read_data;
								state = SEQ;
							} else {
								state = HEADER;
							}
							break;
		}
		case SEQ:		{	seq = read_data;
							rx_buffer[rx_cnt++] = read_data;
							state = MSGID;
							break;
		}
		case MSGID:		{	msg_id = read_data;
							rx_buffer[rx_cnt++] = read_data;
							state = PAYLOADSIZE;
							break;
		}
		case PAYLOADSIZE:{	payload_size = read_data;
							rx_buffer[rx_cnt++] = read_data;
							state = PAYLOAD;
							break;
		}
		case PAYLOAD:	{	payload[payload_cnt++] = read_data;
							rx_buffer[rx_cnt++] = read_data;
							if (payload_cnt == payload_size) {
								state = CHECK;
								payload_cnt = 0;
							}
							break;
		}
		case CHECK:		{	crc[crc_cnt++] = read_data;
							if (crc_cnt == crc_len) {
								crc_cnt = 0;
								state = HEADER;
								if (Check()) {
									return true;
								}
							}
							break;
		}
		default:			break;
	}
	return false;
	// return false;
}
bool eXPlotConnect::Check() {
	uint16_t crc_16 = 0;
	crc_16 = crc[0] + crc[1]*0x100;
	uint16_t modcrc = ModBusCRC16(&rx_buffer[0], rx_cnt);
	if (crc_16 == modcrc)  {
		return true;
	}
	return false;
}
void eXPlotConnect::Decode() {
    switch (msg_id) {
		case EXPLOT_MSGID_VALUETX: {
			rx_msg = new ValueTx((payload_size-4)/4);
			break;
		}
		// case SBUSTEMP_RX_MSGID: {
		// 	rx_msg = new SbusRx();
		// 	break;
		// }
		default: {
			rx_msg = nullptr;
			return;
		}	
				
	}
	rx_msg->Decode(&payload[0]);
}
void eXPlotConnect::Send() {
	protocol_size = 0;
	tx_buffer[protocol_size++] = header;
	tx_buffer[protocol_size++] = tx_seq;
	tx_buffer[protocol_size++] = tx_msg_id;
	tx_buffer[protocol_size++] = tx_payload_size;
	memcpy(&tx_buffer[protocol_size], &tx_payload[0], tx_payload_size);
	protocol_size += tx_payload_size;
	GenerateCheck();
	memcpy(&tx_buffer[protocol_size], &tx_crc[0], crc_len);
	protocol_size += crc_len;
	transmit_func(&tx_buffer[0], protocol_size);
}
uint8_t eXPlotConnect::GenerateCheck() {
	uint16_t _crc;
	crc_len = 2;
	_crc = ModBusCRC16(&rx_buffer[0], tx_payload_size);
	memcpy(&tx_crc[0], &_crc, crc_len);
	return crc_len;
}
bool eXPlotConnect::LaunchSend(msg::Message* msg) {
	if (!is_initalized) return false;
	if (msg == nullptr) return false;

	tx_seq ++;
	tx_msg_id = msg->msg_id;
	tx_address = msg->address;
	msg->Encode(&tx_payload[0]);
	tx_payload_size = msg->GetLen();
	tx_msg = msg;

	Send();
	return true;
}
} /* namespace debugger */
