#pragma once

#include <Message.h>
#include "VisionSensorConnect.h"
namespace sensor {

class PositionMeasure: public msg::Message {
private:
	float distance;
    float degree;			
public:
	const uint8_t msg_id = SENSOR_MSGID_POSITION;

	PositionMeasure();
	virtual ~PositionMeasure();
	virtual void Encode(uint8_t* payload);
	virtual void Decode(uint8_t* payload);
	virtual void UpdataData();
	virtual void Execute();
};


} /* namespace debugger */
