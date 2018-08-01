#pragma once

#include "IBGTimerObject.h"

class BGTimerToken
{
public:
	IBGTimerObject* m_Object;
	int m_TimerEventType;
	std::vector<void*> m_Params;

};

class BGTimer
{
	using TimerToken = BGTimerToken*;
	using TimerQueue = std::priority_queue<std::chrono::milliseconds, std::vector<TimerToken>>;

public:
	BGTimer();
	~BGTimer();

public:
	// 타이머 시스템을 시작합니다.
	bool Start();

	// 타이머 시스템을 종료합니다.
	bool Stop();
	
	/** 타이머에서 동작할 스레드 입니다.*/
	void Run();


private:
	std::shared_mutex m_RunSharedLock;
	bool m_bTimerRunning;

	std::mutex m_queueLock;
	TimerQueue m_queue;


	std::vector<std::thread*> m_vecRunThread;

	/** 스레드에서 동작할 멤버함수(Run)  생성*/
	std::thread* RunSpawn();
};

