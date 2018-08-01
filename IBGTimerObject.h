#pragma once

#include "BGErrorEnum.h"

struct BGTimerParam;

class IBGTimerObject
{
public:
	virtual BG_ERROR::Timer TimerProcess(int type, BGTimerParam params) = 0;
};