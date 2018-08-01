#pragma once
class TestTimerObject : public IBGTimerObject
{
public:
	TestTimerObject();
	~TestTimerObject();

	virtual BG_ERROR::Timer TimerProcess(int type, BGTimerParam params);
};

