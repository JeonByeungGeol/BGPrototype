#include "stdafx.h"
#include "BGLogManager.h"


BGLogManager::BGLogManager()
{
}


BGLogManager::~BGLogManager()
{
}

/**
 * !새로운 로그를 추가하면, 이곳에 추가해야 합니다.
 * 로그 시스템에 필요한 설정을 합니다.
 * logLevel 이 어떤 로그파일에 씌어야 하는지 설정합니다.
 * 필요한 파일 스트림을 생성합니다.
*/
bool BGLogManager::Init()
{
	// 로그레벨 - 로그이름 매칭
	m_logLevelLogNameMap.insert(std::make_pair(ELogLevel::NONE, "NONE"));
	m_logLevelLogNameMap.insert(std::make_pair(ELogLevel::TRACE, "TRACE"));
	m_logLevelLogNameMap.insert(std::make_pair(ELogLevel::DEBUG, "DEBUG"));
	m_logLevelLogNameMap.insert(std::make_pair(ELogLevel::INFO, "INFO"));
	m_logLevelLogNameMap.insert(std::make_pair(ELogLevel::WARNING, "WARNING"));
	m_logLevelLogNameMap.insert(std::make_pair(ELogLevel::ERROR, "ERROR"));
	m_logLevelLogNameMap.insert(std::make_pair(ELogLevel::FATAL, "FATAL"));
	//m_logLevelLogNameMap.insert(std::make_pair(ELogLevel::EXTRACT_DATA_1, "EXTRACT_DATA_1"));

	// 폴더이름 - 파일스트림 매칭
	m_forderNameFileStreamMap.insert(std::make_pair("log", std::fstream{}));	// default
	m_forderNameFileStreamMap.insert(std::make_pair("err", std::fstream{}));
	//m_forderNameFileStreamMap.insert(std::make_pair("extract_data_1", std::fstream{}));
	
	// 로그레벨 - 폴더이름 매칭 (매칭 안되있는 건 default폴더)
	m_logLevelForderNameMap.insert(std::make_pair(ELogLevel::NONE, "err"));
	// m_logLevelForderNameMap.insert(std::make_pair(ELogLevel::TRACE, "TRACE"));
	// m_logLevelForderNameMap.insert(std::make_pair(ELogLevel::DEBUG, "DEBUG"));
	// m_logLevelForderNameMap.insert(std::make_pair(ELogLevel::INFO, "INFO"));
	m_logLevelForderNameMap.insert(std::make_pair(ELogLevel::WARNING, "err"));
	m_logLevelForderNameMap.insert(std::make_pair(ELogLevel::ERROR, "err"));
	m_logLevelForderNameMap.insert(std::make_pair(ELogLevel::FATAL, "err"));
	//m_logLevelForderNameMap.insert(std::make_pair(ELogLevel::EXTRACT_DATA_1, "extract_data_1"));

	return false;
}

/**
 * 이 함수를 호출하면 로그시스템이 시작합니다.
 * 로그를 찍는 전용 스레드를 만들어 동작
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

	for (auto iter = m_forderNameFileStreamMap.begin(); iter != m_forderNameFileStreamMap.end(); iter++) {
		if ((iter->second).is_open()) 
			(iter->second).close();		
	}

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
 * 기록해야 하는 로그인지 로그레벨로 검사합니다.
 * bg_config.ini파일에 LogLevel의 값보다 커야
 * 실제 로그를 기록할 수 있습니다.
*/
bool BGLogManager::CheckLogLevel(BGLog & log)
{
	return (log.GetLevel() > g_Config.GetInt("LogLevel"));
}

/**
 * 로그레벨이 기본로그인지 검사합니다.
 * 기본로그는 TRACE, DEBUG, INFO, WANING, ERROR, FATAL
 * 6가지가 존재합니다.
*/
bool BGLogManager::IsBasicLogLevel(BGLog &log)
{
	auto level = log.GetLevel();
	switch(level) {
		case ELogLevel::TRACE:
		case ELogLevel::DEBUG:
		case ELogLevel::INFO:
		case ELogLevel::WARNING:
		case ELogLevel::ERROR:
		case ELogLevel::FATAL:
			return true;
		default:
			return false;
	}
}


/**
 * Queue에서 로그를 꺼냅니다.
 * 꺼낸 로그는 Valid를 호출해서
 * 유효한 로그인지 확인한 후에 사용합니다.
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
 * 로그를 Queue에 넣습니다.
 * 이후 전용스레드에서 하나씩 꺼내 처리합니다.
*/
void BGLogManager::Push(BGLog& log)
{
	m_queueLock.lock();
	m_queue.push(log);
	m_queueLock.unlock();
}

void BGLogManager::Write(BGLog &log)
{
	if (IsBasicLogLevel(log)) {
		log.Write(m_defaultLogFileStream);
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

		// 로그 파일 업데이트

		// 로그 쓰기
		pLogMgr->Write(log);				
	}

	return;
}
