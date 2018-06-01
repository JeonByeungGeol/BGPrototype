#pragma once

#include "BGLog.h"
/**
 * <pre>
 * �α׸� �����Ѵ�.
 *
 * ���
 * 1. �α׸� ���� ���� �����Ѵ�. (trace, debug, info, err, fatal...)
 * 2. �α������� Ŀ���� �ʵ��� �Ⱓ���� ���ο� ����/������ ���⵵�� �Ѵ�.
 * 3. ���� ����� �α׸� ���� ���� �Ѵ�. (extract1, extract2, ...)
 *
 * ! ���ο� �α� Ÿ�� �߰� �� (������ �����)
 * BGLogEnum�� �α׷��� �߰� �Ŀ� Init���� �������ָ� �ȴ�.
 *
 * </pre>
*/

class BGLogManager
{
	using LogQueue = std::queue<BGLog>;

	using LogLevelLogNameMap = std::map<ELogLevel, std::string>;
	using ForderNameFileStreamMap = std::map<std::string, std::fstream*>;
	using LogLevelForderNameMap = std::map<ELogLevel, std::string>;
	
public:
	BGLogManager();
	~BGLogManager();

public:
	// �α� �ý��ۿ� �ʿ��� ������ �մϴ�.
	bool Init();

	// �α� �ý����� �����մϴ�.
	bool Start();

	// �α� �ý����� �����մϴ�.
	bool Stop();
	
	// �α� �ý��� �����û�α����� Ȯ���մϴ�.
	bool IsStopRequest(BGLog&);

	// �α� ������ �˻��մϴ�.
	bool CheckLogLevel(BGLog&);

	// �α� ������ �⺻�������� �˻��մϴ�.(TRACE, DEBUG, INFO, WANING, ERROR, FATAL)
	bool IsBasicLogLevel(BGLog&);

	// queue���� �α׸� �ϳ� �����ϴ�.
	BGLog& Pick();

	// queue�� �α׸� �ֽ��ϴ�.
	void Push(BGLog&);

	// log�� ����մϴ�.
	void Write(BGLog&);
	
	bool CheckLogFileNameAndRenew();

	void RenewLogFileStream();

	///////////////////////////////
	// �α� ���� ������ �Լ� �Դϴ�
	///////////////////////////////
	static void Run(BGLogManager*);

private:
	std::mutex m_queueLock;
	LogQueue m_queue;

	std::thread* m_pRunThread;
	
	struct tm m_lastCreateFileName;

	std::vector<std::pair<std::string, std::fstream*>> m_logFileStreamVec;

	LogLevelLogNameMap m_logLevelLogNameMap;
	ForderNameFileStreamMap m_forderNameFileStreamMap;
	LogLevelForderNameMap m_logLevelForderNameMap;
};

