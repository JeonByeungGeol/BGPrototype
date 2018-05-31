#pragma once

#include "BGLog.h"
/**
 * <pre>
 * 로그를 관리한다.
 *
 * 기능
 * 1. 로그를 레벨 별로 관리한다. (trace, debug, info, err, fatal...)
 * 2. 로그파일이 커지지 않도록 기간마다 새로운 폴더/파일이 생기도록 한다.
 * 3. 정보 추출용 로그를 따로 관리 한다. (extract1, extract2, ...)
 
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
	
	bool IsStopRequest(BGLog&);

	BGLog& Pick();

	void Push(BGLog&);


	void Write(BGLog&);

	static void Run(BGLogManager*);



private:
	std::mutex m_queueLock;
	LogQueue m_queue;

	std::thread* m_pRunThread;
	
};

