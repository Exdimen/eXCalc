/*
 * eXPlotMsgs.cpp
 *
 *  Created on: Nov 22, 2022
 *      Author: Exdimen
 */

#include "VisionSensorMsgs.h"
#include "eXPlot.h"
namespace sensor {

PositionMeasure::PositionMeasure() {
	distance = 0;
    degree = 0;
}

PositionMeasure::~PositionMeasure() {
}
void PositionMeasure::Decode(uint8_t* payload) {
	// len_cnt = 0;
	// memcpy				(&timestamp, &payload[len_cnt], sizeof(timestamp));
	// len_cnt += sizeof	(timestamp);
	// memcpy				(value,		 &payload[len_cnt], len * sizeof(float));
	// len_cnt += len * sizeof(float);
}
void PositionMeasure::Encode(uint8_t* payload) {
	// len_cnt = 0;
	// memcpy				(&payload[len_cnt], &timestamp, sizeof(timestamp));
	// len_cnt += sizeof	(timestamp);
	// memcpy				(&payload[len_cnt], value, len * sizeof(float));
	// len_cnt += len * sizeof(float);
}
// float temp;
void PositionMeasure::UpdataData() {
	// if (len == 1) {
	// 	value[0] = temp;
	// }
	// timestamp = millis();
}


void PositionMeasure::Execute() {
	// pdata.chennal_num = len;
	// for (int i = N-1; i > 0; i--) {
	// 	if (len >= 1) {
	// 		pdata.x[i] = pdata.x[i-1];
	// 	}
	// 	if (len >= 2) {
	// 		pdata.y[i] = pdata.y[i-1];
	// 	}
	// 	if (len >= 3) {
	// 		pdata.p[i] = pdata.p[i-1];
	// 	}
	// 	if (len >= 4) {
	// 		pdata.q[i] = pdata.q[i-1];
	// 	}
	// 	for (int j = 0 ; j< 4; j++) {
	// 		pdata.t[j][i] = pdata.t[j][i-1];
	// 	}
	// }
	// pdata.t[0][0] 	= (float) timestamp;			// us
	// pdata.t[1][0] 	= (float) pdata.t[0][0]/1000;	// ms
	// pdata.t[2][0] 	= (float) pdata.t[1][0]/1000;	// s
	// pdata.t[3][0]	= (float) pdata.t[2][0]/60;		// min

	// if (len >= 1) {
	// 	pdata.x[0] = value[0];
	// }
	// if (len >= 2) {
	// 	pdata.y[0] = value[1];
	// }
	// if (len >= 3) {
	// 	pdata.p[0] = value[2];
	// }
	// if (len >= 4) {
	// 	pdata.q[0] = value[3];
	// }
}

} /* namespace debugger */


