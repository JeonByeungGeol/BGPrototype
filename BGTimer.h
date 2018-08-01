#pragma once
class BGTimer
{
	using TimerToken = int*;
	using TimerQueue = std::priority_queue<std::chrono::milliseconds, std::vector<TimerToken>>;

public:
	BGTimer();
	~BGTimer();

public:
	// Ÿ�̸� �ý����� �����մϴ�.
	bool Start();

	// Ÿ�̸� �ý����� �����մϴ�.
	bool Stop();
	
	/** Ÿ�̸ӿ��� ������ ������ �Դϴ�.*/
	void Run();


private:
	std::shared_mutex m_RunSharedLock;
	bool m_bTimerRunning;

	std::mutex m_queueLock;
	TimerQueue m_queue;


	std::vector<std::thread*> m_vecRunThread;

	/** �����忡�� ������ ����Լ�(Run)  ����*/
	std::thread* RunSpawn();
};

