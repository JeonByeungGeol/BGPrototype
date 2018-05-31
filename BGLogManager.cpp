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
 * �α� �ý��� ���� ��û �α׸� ���뽺���忡 �����Ŀ�
 * �α����� �����尡 Ȯ���ϸ�, �����尡 ����� ���� Ȯ���� �ڿ�
 * true�� �����մϴ�.
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
 * �α� �ý����� �������Ḧ ��û�ϴ� �αװ� ���Դ��� Ȯ���մϴ�.
 * ���� ��û �α״� [�α׷��� : INFO, ���� : "STOP"] �Դϴ�.
 * �α� ���� �����忡���� ȣ�� �մϴ�.
 * true�� �����ϸ�, �α� ���� ������� ����˴ϴ�.
*/
bool BGLogManager::IsStopRequest(BGLog& log)
{
	return log.IsStopRequest();
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

		// �α� ���� ������Ʈ

		// �α� ����
		pLogMgr->Write(log);
				
	}

	return;
}
