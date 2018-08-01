#include "stdafx.h"
#include "BGTimer.h"


BGTimer::BGTimer()
	:m_bTimerRunning(false)
{
	m_vecRunThread.reserve(BG_TIMER_THREAD_NUM);
}

BGTimer::~BGTimer()
{
}

bool BGTimer::Start()
{
	m_bTimerRunning = true;

	if (!m_vecRunThread.empty())
		return false;

	for (int i = 0; i < BG_TIMER_THREAD_NUM; i++)
		m_vecRunThread.push_back( RunSpawn() );

	return true;
}

bool BGTimer::Stop()
{
	m_RunSharedLock.lock();
	m_bTimerRunning = false;
	m_RunSharedLock.unlock();

	for (auto th : m_vecRunThread) {
		th->join();
		delete th;
		th = nullptr;
	}

	m_vecRunThread.clear();

	return true;
}

std::thread* BGTimer::RunSpawn()
{
	return new std::thread{ [this]() { this->Run(); } };
}


void BGTimer::Run()
{
	BG_LOG_TRACE("timerthread run");

	while (1) {	
		m_RunSharedLock.lock_shared();
		if (m_bTimerRunning == false) {
			m_RunSharedLock.unlock_shared();
			break;
		}
		m_RunSharedLock.unlock_shared();

		m_queueLock.lock();		
		if (m_queue.empty()) {
			m_queueLock.unlock();
			continue;
		}

		TimerToken token = m_queue.top();
		m_queue.pop();
		m_queueLock.unlock();

		// token으로 작업 시작!
		if (token == nullptr) {
			BG_LOG_ERROR("1");
			continue;
		}

		BG_ERROR::Timer error = token->m_Object->TimerProcess(token);
		if (BG_ERROR::Timer::BG_NONE != error) {
			BG_LOG_ERROR("[error=%d]", BG_ERROR::ErrorCode(error));
		}
	}

	BG_LOG_TRACE("timerthread exit");
}