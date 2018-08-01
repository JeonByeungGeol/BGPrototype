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
		// 타이머가 동작중인지 확인
		m_RunSharedLock.lock_shared();
		if (m_bTimerRunning == false) {
			m_RunSharedLock.unlock_shared();
			break;
		}
		m_RunSharedLock.unlock_shared();

		// 작업이 있는지 확인
		m_queueLock.lock();		
		if (m_queue.empty()) {
			m_queueLock.unlock();
			continue;
		}

		// 가장 빠른 작업 실행 시간이 되었는지 확인
		std::chrono::system_clock::duration duEpoch = std::chrono::system_clock::now().time_since_epoch();
		__int64 nowTimePoint = std::chrono::duration_cast<std::chrono::milliseconds>(duEpoch).count();
		TimerToken token = m_queue.top();
		if (nowTimePoint < token->m_ExecTime) {
			m_queueLock.unlock();
			continue;
		}

		m_queue.pop();
		m_queueLock.unlock();

		// token으로 작업 시작
		if (token == nullptr) {
			BG_LOG_ERROR("1");
			continue;
		}

		BG_ERROR::Timer error = token->m_Object->TimerProcess(token->m_TimerEventType, token->m_Params);
		if (BG_ERROR::Timer::BG_NONE != error) {
			BG_LOG_ERROR("[error=%d]", BG_ERROR::ErrorCode(error));
		}

		// 사용한 token 삭제
		delete token;
		token = nullptr;
	}

	BG_LOG_TRACE("timerthread exit");
}

void BGTimer::Push(__int64 milSec, IBGTimerObject* pObject, int type, BGTimerParam params)
{
	std::chrono::system_clock::duration duEpoch = std::chrono::system_clock::now().time_since_epoch();
	__int64 nowTimePoint = std::chrono::duration_cast<std::chrono::milliseconds>(duEpoch).count();

	m_queueLock.lock();
	m_queue.push(new BGTimerToken{
		nowTimePoint + (milSec)
		, pObject
		, type
		, params});
	m_queueLock.unlock();
}
