#pragma once

#include "IBGTimerObject.h"
#include "TestTimerObject.h"

struct BGTimerParam
{
	void* param1;
	void* param2;
	void* param3;
	void* param4;
};

class BGTimerToken
{
public:
	BGTimerToken(__int64 milSec, IBGTimerObject* pObject, int type, BGTimerParam params)
		: m_ExecTime(milSec), m_Object(pObject), m_TimerEventType(type), m_Params(params)
	{
	}

public:
	__int64 m_ExecTime;

	IBGTimerObject* m_Object;
	int m_TimerEventType;
	BGTimerParam m_Params;
};

struct Compare {
	bool operator()(const BGTimerToken* ref1, const BGTimerToken* ref2)
	{
		return ref1->m_ExecTime > ref2->m_ExecTime;
	}
};

class BGTimer
{
	using TimerToken = BGTimerToken*;
	using TimerQueue = std::priority_queue < TimerToken, std::vector<TimerToken>, Compare> ;

public:
	BGTimer();
	~BGTimer();

public:
	// 타이머 시스템을 시작합니다.
	bool Start();

	// 타이머 시스템을 종료합니다.
	bool Stop();
		
	/** 타이머 작업 추가 */
	void Push(__int64, IBGTimerObject*, int, BGTimerParam);


private:
	/** 타이머가 동작하고 있는지 나타내는 값과 그 값을 동기화 하기 위한 뮤텍스*/
	std::shared_mutex m_RunSharedLock;
	bool m_bTimerRunning;

	/** 특정 시간에 작업해야 하는 일을 가지고 있는 큐와 동기화 하기 위한 뮤텍스*/
	std::mutex m_queueLock;
	TimerQueue m_queue;

	/** 실질적으로 동작하는 스레드 함수의 포인트*/
	std::vector<std::thread*> m_vecRunThread;
	
	/** 스레드에서 동작할 멤버함수(Run)  생성*/
	std::thread* RunSpawn();

	/** 타이머에서 동작할 스레드 입니다. */
	void Run();
};

