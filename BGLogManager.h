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
 
 * </pre>
*/

class BGLogManager
{
	using LogQueue = std::queue<BGLog>;
	
public:
	BGLogManager();
	~BGLogManager();

public:
	bool Start();
	bool Stop();
	
	bool IsStop();

	BGLog& Pick();

	void Write(BGLog&);

	static void Run(BGLogManager*);



private:
	std::mutex m_queueLock;
	LogQueue m_queue;

	std::thread* m_pRunThread;
	
};

