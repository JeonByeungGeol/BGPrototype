#include "stdafx.h"
#include "BGLogManager.h"


BGLogManager::BGLogManager()
{
}


BGLogManager::~BGLogManager()
{
}

/**
 * !���ο� �α׸� �߰��ϸ�, �̰��� �߰��ؾ� �մϴ�.
 * �α� �ý��ۿ� �ʿ��� ������ �մϴ�.
 * logLevel �� � �α����Ͽ� ����� �ϴ��� �����մϴ�.
 * �ʿ��� ���� ��Ʈ���� �����մϴ�.
*/
bool BGLogManager::Init()
{
	// �α׷��� - �α��̸� ��Ī
	m_logLevelLogNameMap.insert(std::make_pair(ELogLevel::NONE, "NONE"));
	m_logLevelLogNameMap.insert(std::make_pair(ELogLevel::TRACE, "TRACE"));
	m_logLevelLogNameMap.insert(std::make_pair(ELogLevel::DEBUG, "DEBUG"));
	m_logLevelLogNameMap.insert(std::make_pair(ELogLevel::INFO, "INFO"));
	m_logLevelLogNameMap.insert(std::make_pair(ELogLevel::WARNING, "WARNING"));
	m_logLevelLogNameMap.insert(std::make_pair(ELogLevel::ERROR, "ERROR"));
	m_logLevelLogNameMap.insert(std::make_pair(ELogLevel::FATAL, "FATAL"));
	//m_logLevelLogNameMap.insert(std::make_pair(ELogLevel::EXTRACT_DATA, "EXTRACT_DATA"));

	// �����̸� - ���Ͻ�Ʈ�� ��Ī
	m_forderNameFileStreamMap.insert(std::make_pair("log", std::fstream{}));	// default
	m_forderNameFileStreamMap.insert(std::make_pair("err", std::fstream{}));
	//m_forderNameFileStreamMap.insert(std::make_pair("extract_data", std::fstream{}));
	
	// �α׷��� - �����̸� ��Ī (��Ī �ȵ��ִ� �� default����)
	m_logLevelForderNameMap.insert(std::make_pair(ELogLevel::NONE, "err"));
	// m_logLevelForderNameMap.insert(std::make_pair(ELogLevel::TRACE, "TRACE"));
	// m_logLevelForderNameMap.insert(std::make_pair(ELogLevel::DEBUG, "DEBUG"));
	// m_logLevelForderNameMap.insert(std::make_pair(ELogLevel::INFO, "INFO"));
	m_logLevelForderNameMap.insert(std::make_pair(ELogLevel::WARNING, "err"));
	m_logLevelForderNameMap.insert(std::make_pair(ELogLevel::ERROR, "err"));
	m_logLevelForderNameMap.insert(std::make_pair(ELogLevel::FATAL, "err"));
	//m_logLevelForderNameMap.insert(std::make_pair(ELogLevel::EXTRACT_DATA, "EXTRACT_DATA"));

	return false;
}

/**
 * �� �Լ��� ȣ���ϸ� �α׽ý����� �����մϴ�.
 * �α׸� ��� ���� �����带 ����� ����
*/
bool BGLogManager::Start()
{
	if (!Init())
		return false;

	if (m_pRunThread != nullptr)
		return false;

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

/**
 * �α׸� Queue�� �ֽ��ϴ�.
 * ���� ���뽺���忡�� �ϳ��� ���� ó���մϴ�.
*/
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
