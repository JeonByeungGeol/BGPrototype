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
 * 로그 시스템 종료 요청 로그를 전용스레드에 보낸후에
 * 로그전용 스레드가 확인하면, 스레드가 종료된 것을 확인한 뒤에
 * true를 리턴합니다.
*/
bool BGLogManager::Stop()
{
	Push(BGLog{ ELogLevel::INFO, "STOP" });

	m_pRunThread->join();

	delete m_pRunThread;
	m_pRunThread = nullptr;

	return false;
}

/**
 * 로그 시스템이 동작종료를 요청하는 로그가 들어왔는지 확인합니다.
 * 종료 요청 로그는 [로그레벨 : INFO, 내용 : "STOP"] 입니다.
 * 로그 전용 스레드에서만 호출 합니다.
 * true를 리턴하면, 로그 전용 스레드는 종료됩니다.
*/
bool BGLogManager::IsStopRequest(BGLog& log)
{
	return log.IsStopRequest();
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

void BGLogManager::Push(BGLog& log)
{
	m_queueLock.lock();
	m_queue.push(log);
	m_queueLock.unlock();
}

void BGLogManager::Write(BGLog &log)
{
}


void BGLogManager::Run(BGLogManager* pLogMgr)
{
	BGLog log;

	while (true)
	{		
		log = pLogMgr->Pick();
		if (!log.Valid())
			continue;
		
		if (pLogMgr->IsStopRequest(log)) {
			return;
		}

		// 로그 파일 업데이트

		// 로그 쓰기
		pLogMgr->Write(log);
				
	}

	return;
}
