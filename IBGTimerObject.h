#pragma once

#include "BGErrorEnum.h"

class BGTimerToken;

class IBGTimerObject
{
public:
	virtual BG_ERROR::Timer TimerProcess(BGTimerToken* token) = 0;
};