#include "stdafx.h"
#include "TestTimerObject.h"


TestTimerObject::TestTimerObject()
{
}


TestTimerObject::~TestTimerObject()
{
}

BG_ERROR::Timer TestTimerObject::TimerProcess(int type, BGTimerParam params)
{
	switch (type) {
	case 0:
		std::cout << "exec timer 0 type" << std::endl;
		break;

	case 1:
		std::cout << "exec timer 1 type" << std::endl;
		break;

	default:
		return BG_ERROR::Timer::BG_INVALID_TIMER_EVENT_TYPE;
	}
	
	return BG_ERROR::Timer::BG_NONE;
}
