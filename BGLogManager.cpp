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
	// ������ �α����� ���� �ð�
	m_lastCreateFileName.tm_hour = -1;

	//////////////////////////////////////////////////////////////////////////////////////////////////
	// �α׷��� - �α��̸� ��Ī
	m_logLevelLogNameMap.insert(std::make_pair(ELogLevel::BG_NONE, "NONE"));
	m_logLevelLogNameMap.insert(std::make_pair(ELogLevel::BG_TRACE, "TRACE"));
	m_logLevelLogNameMap.insert(std::make_pair(ELogLevel::BG_DEBUG, "DEBUG"));
	m_logLevelLogNameMap.insert(std::make_pair(ELogLevel::BG_INFO, "INFO"));
	m_logLevelLogNameMap.insert(std::make_pair(ELogLevel::BG_WARNING, "WARNING"));
	m_logLevelLogNameMap.insert(std::make_pair(ELogLevel::BG_ERROR, "ERROR"));
	m_logLevelLogNameMap.insert(std::make_pair(ELogLevel::BG_FATAL, "FATAL"));	
	//////////////////////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////////////////////////
	// �����̸� - ���Ͻ�Ʈ�� ��Ī
	std::fstream* pFileStream{ nullptr };
	
	// default �α� ����
	pFileStream = new std::fstream;
	m_logFileStreamVec.push_back(std::make_pair("log\\", pFileStream));
	m_forderNameFileStreamMap.insert(std::make_pair("log", pFileStream));	

	pFileStream = new std::fstream;
	m_logFileStreamVec.push_back(std::make_pair("err\\", pFileStream));
	m_forderNameFileStreamMap.insert(std::make_pair("err", pFileStream));	
	//////////////////////////////////////////////////////////////////////////////////////////////////
	
	//////////////////////////////////////////////////////////////////////////////////////////////////
	// �α׷��� - �����̸� ��Ī (��Ī �ȵ��ִ� �� default����)
	m_logLevelForderNameMap.insert(std::make_pair(ELogLevel::BG_NONE, "err"));
	m_logLevelForderNameMap.insert(std::make_pair(ELogLevel::BG_WARNING, "err"));
	m_logLevelForderNameMap.insert(std::make_pair(ELogLevel::BG_ERROR, "err"));
	m_logLevelForderNameMap.insert(std::make_pair(ELogLevel::BG_FATAL, "err"));	
	/////////////////////////////////////////////////////////////////////////////////////////////////

	/**
	 * ���ο� �α� Ÿ�� �߰���
	 * 1��
	 * m_logLevelLogNameMap.insert(std::make_pair(ELogLevel::BG_EXTRACT_DATA_1, "EXTRACT_DATA_1"));
	 * 2��
	 * pFileStream = new std::fstream;
	 * m_logFileStreamVec.push_back(std::make_pair("extract_data_1", pFileStream));
	 * m_forderNameFileStreamMap.insert(std::make_pair("extract_data_1", pFileStream));
	 * 3��
	 * m_logLevelForderNameMap.insert(std::make_pair(ELogLevel::BG_EXTRACT_DATA_1, "extract_data_1"));
	*/
	return true;
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
	Push(BGLog{ ELogLevel::BG_INFO, "STOP" });

	m_pRunThread->join();

	delete m_pRunThread;
	m_pRunThread = nullptr;

	for (auto fStream : m_logFileStreamVec) {
		if (fStream.second->is_open())
			fStream.second->close();
		delete fStream.second;
		fStream.second = nullptr;
	}

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
 * ����ؾ� �ϴ� �α����� �α׷����� �˻��մϴ�.
 * bg_config.ini���Ͽ� LogLevel�� ������ Ŀ��
 * ���� �α׸� ����� �� �ֽ��ϴ�.
*/
bool BGLogManager::CheckLogLevel(BGLog & log)
{
	return (static_cast<int>(log.GetLevel()) > g_Config.GetInt("LogLevel"));
}

/**
 * �α׷����� �⺻�α����� �˻��մϴ�.
 * �⺻�α״� TRACE, DEBUG, INFO, WANING, ERROR, FATAL
 * 6������ �����մϴ�.
*/
bool BGLogManager::IsBasicLogLevel(BGLog &log)
{
	auto level = log.GetLevel();
	switch(level) {
		case ELogLevel::BG_TRACE:
		case ELogLevel::BG_DEBUG:
		case ELogLevel::BG_INFO:
		case ELogLevel::BG_WARNING:
		case ELogLevel::BG_ERROR:
		case ELogLevel::BG_FATAL:
			return true;
		default:
			return false;
	}
}


/**
 * Queue���� �α׸� �����ϴ�.
 * ���� �α״� Valid�� ȣ���ؼ�
 * ��ȿ�� �α����� Ȯ���� �Ŀ� ����մϴ�.
*/
BGLog & BGLogManager::Pick()
{
	m_queueLock.lock();		//--*--*--*--*--*--*--

	BGLog& log = m_queue.front();

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
	if (!CheckLogFileNameAndRenew()) {
		RenewLogFileStream();
	}

	std::fstream* pDefault{ nullptr };
	if (IsBasicLogLevel(log)) {
		pDefault = m_logFileStreamVec[0].second;
	}

	std::fstream* pSpecific{ nullptr };
	auto iter1 = m_logLevelForderNameMap.find(log.GetLevel());
	if (iter1 != m_logLevelForderNameMap.end()) {
		auto iter2 = m_forderNameFileStreamMap.find(iter1->second);
		if (iter2 != m_forderNameFileStreamMap.end())
			pSpecific = iter2->second;		
	}
	
	if (pDefault)
		log.Write(pDefault);
	
	if (pSpecific)
		log.Write(pSpecific);
}

bool BGLogManager::CheckLogFileNameAndRenew()
{
	struct tm ltm;
	time_t t = time(NULL);
	localtime_s(&ltm, &t);

	if (ltm.tm_hour != m_lastCreateFileName.tm_hour) {
		m_lastCreateFileName = ltm;
		return false;
	}

	return true;
}

void BGLogManager::RenewLogFileStream()
{
	char timestr[32];
	sprintf_s(timestr, "%04d-%02d-%02d_%02d.LOG", m_lastCreateFileName.tm_year + 1900,
		m_lastCreateFileName.tm_mon + 1,
		m_lastCreateFileName.tm_mday,
		m_lastCreateFileName.tm_hour);

	for (auto fileStream : m_logFileStreamVec) {

		if (fileStream.second->is_open())
			fileStream.second->close();

		std::string fileName{ fileStream.first };
		fileName.append(timestr);
		fileStream.second->open(fileName, std::ios::app);

	}
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
