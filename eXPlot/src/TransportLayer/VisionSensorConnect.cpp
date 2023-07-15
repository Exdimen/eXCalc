

#include "VisionSensorConnect.h"
#include "VisionSensorMsgs.h"
#include "string.h"

#include "CRCHub.h"
namespace sensor {


VisionSensorConnect::VisionSensorConnect(ConnectType _cont_type) {
	cont_type = _cont_type;
	crc_len = 2;
}


VisionSensorConnect::~VisionSensorConnect() {

}

bool VisionSensorConnect::Parse() {
	switch (state) {
		case HEADER:	{	cnt = 0;
							if (read_data == header) {
								buffer[cnt++] = read_data;
								state = SEQ;
							} else {
								state = HEADER;
							}
							break;
		}
		case SEQ:		{	seq = read_data;
							buffer[cnt++] = read_data;
							state = MSGID;
							break;
		}
		case MSGID:		{	msg_id = read_data;
							buffer[cnt++] = read_data;
							state = PAYLOADSIZE;
							break;
		}
		case PAYLOADSIZE:{	payload_size = read_data;
							buffer[cnt++] = read_data;
							state = PAYLOAD;
							break;
		}
		case PAYLOAD:	{	payload[payload_cnt++] = read_data;
							buffer[cnt++] = read_data;
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
bool VisionSensorConnect::Check() {
	uint16_t crc_16 = 0;
	crc_16 = crc[0] + crc[1]*0x100;
	uint16_t modcrc = ModBusCRC16(&buffer[0], cnt);
	if (crc_16 == modcrc)  {
		return true;
	}
	return false;
}
void VisionSensorConnect::Decode() {
    switch (msg_id) {
		case SENSOR_MSGID_POSITION: {
			rx_msg = new PositionMeasure();
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
void VisionSensorConnect::Send() {
	protocol_size = 0;
	buffer[protocol_size++] = header;
	buffer[protocol_size++] = seq;
	buffer[protocol_size++] = msg_id;
	buffer[protocol_size++] = payload_size;
	memcpy(&buffer[protocol_size], &payload[0], payload_size);
	protocol_size += payload_size;
	GenerateCheck();
	memcpy(&buffer[protocol_size], &crc[0], crc_len);
	protocol_size += crc_len;
	transmit_func(&buffer[0], protocol_size);
}
uint8_t VisionSensorConnect::GenerateCheck() {
	uint16_t _crc;
	crc_len = 2;
	_crc = ModBusCRC16(&buffer[0], payload_size);
	memcpy(&crc[0], &_crc, crc_len);
	return crc_len;
}
bool VisionSensorConnect::LaunchSend(msg::Message* msg) {
	if (!is_initalized) return false;
	if (msg == nullptr) return false;

	seq ++;
	msg_id = msg->msg_id;
	address = msg->address;
	msg->Encode(&payload[0]);
	payload_size = msg->GetLen();
	tx_msg = msg;

	Send();
	return true;
}
} /* namespace debugger */
