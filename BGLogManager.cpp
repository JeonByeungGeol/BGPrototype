#include "stdafx.h"
#include "BGLogManager.h"


BGLogManager::BGLogManager()
{
}


BGLogManager::~BGLogManager()
{
}

/**
 * �� �Լ��� ȣ���ϸ� �α׽ý����� �����մϴ�.
 * �α׸� ��� ���� �����带 ����� ����
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
 * �� �Լ��� ȣ���ϸ� �α׽ý����� �����մϴ�.
 * �α� ��� ���� �����带 �����Ų �Ŀ� �����մϴ�.
*/
bool BGLogManager::Stop()
{
	

	return false;
}

/**
 * �α� �ý����� �����ؾ��ϴ����� �˷��ݴϴ�.
 * �ʱ⿡�� true�� ����
 * Startȣ�� ���Ŀ��� false ����
 * Stopȣ�� ���Ŀ��� true ����
*/
bool BGLogManager::IsStop()
{
	return false;
}

/**
 * Queue���� �α׸� �����ϴ�.
 * ���� �α״� Valid�� ȣ���ؼ�
 * ��ȿ�� �α����� Ȯ���� �Ŀ� ����մϴ�.
*/
BGLog & BGLogManager::Pick()
{
	// �α� ����
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
		
		// �α� ���� ������Ʈ

		// �α� ����
		pLogMgr->Write(log);
				
	}

	return;
}
