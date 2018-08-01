#pragma once

#include "IBGTimerObject.h"

class BGTimerToken
{
public:
	BGTimerToken(IBGTimerObject* pObject, int type, std::vector<void*> params)
		: m_Object(pObject), m_TimerEventType(type), m_Params(params)
	{}

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
	// Ÿ�̸� �ý����� �����մϴ�.
	bool Start();

	// Ÿ�̸� �ý����� �����մϴ�.
	bool Stop();
	
	

	/** Ÿ�̸� �۾� �߰� */
	void Push(IBGTimerObject*, int, std::vector<void*>);

private:
	std::shared_mutex m_RunSharedLock;
	bool m_bTimerRunning;

	std::mutex m_queueLock;
	TimerQueue m_queue;


	std::vector<std::thread*> m_vecRunThread;

	/** �����忡�� ������ ����Լ�(Run)  ����*/
	std::thread* RunSpawn();

	/** Ÿ�̸ӿ��� ������ ������ �Դϴ�. */
	void Run();
};

