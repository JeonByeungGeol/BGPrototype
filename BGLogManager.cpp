#include "stdafx.h"
#include "BGLogManager.h"


BGLogManager::BGLogManager()
{
}


BGLogManager::~BGLogManager()
{
}

/**
 * 이 함수를 호출하면 로그시스템이 시작합니다.
 * 로그를 찍는 전용 스레드를 만들어 동작
*/
bool BGLogManager::Start()
{
	if (m_pRunThread != nullptr) {

		return false;
	}

	m_pRunThread = new std::thread{BGLogManager::Run, this};


	return true;
}

/**
 * 이 함수를 호출하면 로그시스템이 종료합니다.
 * 로그 찍는 전용 스레드를 종료시킨 후에 삭제합니다.
*/
bool BGLogManager::Stop()
{
	

	return false;
}

/**
 * 로그 시스템이 동작해야하는지를 알려줍니다.
 * 초기에는 true를 리턴
 * Start호출 이후에는 false 리턴
 * Stop호출 이후에는 true 리턴
*/
bool BGLogManager::IsStop()
{
	return false;
}

/**
 * Queue에서 로그를 꺼냅니다.
 * 꺼낸 로그는 Valid를 호출해서
 * 유효한 로그인지 확인한 후에 사용합니다.
*/
BGLog & BGLogManager::Pick()
{
	// 로그 꺼냄
	BGLog log;

	m_queueLock.lock();		//--*--*--*--*--*--*--

	log = m_queue.front();

	if (!log.Valid()) {
		m_queueLock.unlock();	//**-**-**-**-**-**
		return log;
	}

	m_queue.pop();
	m_queueLock.unlock();	//**-**-**-**-**-**

	return log;
}

void BGLogManager::Write(BGLog &log)
{
}


void BGLogManager::Run(BGLogManager* pLogMgr)
{
	BGLog log;

	while (true)
	{
		if (pLogMgr->IsStop())
			return;
		
		log = pLogMgr->Pick();
		if (!log.Valid())
			continue;
		
		// 로그 파일 업데이트

		// 로그 쓰기
		pLogMgr->Write(log);
				
	}

	return;
}
