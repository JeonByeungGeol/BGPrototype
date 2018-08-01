#pragma once

#include "BGErrorEnum.h"


class IBGTimerObject
{
public:
	virtual BG_ERROR::Timer TimerProcess(int type, std::vector<void*> params) = 0;
};