#include "CRCHub.h"

uint16_t ModBusCRC16(uint8_t* data,uint16_t len) {
	uint16_t i,j,tmp,CRC16;
	CRC16 = 0xFFFF;
	for (i = 0; i<len; i++) {
		CRC16 ^= data[i];
		for (j = 0; j < 8; j++) {
			tmp = (uint16_t)(CRC16 & 0x0001);
			CRC16 >>= 1;
			if (tmp == 1) {
				CRC16 ^= 0xA001;
			}
		}
	}
	return CRC16;
}