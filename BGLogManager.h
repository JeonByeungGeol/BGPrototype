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
	// 로그 시스템을 시작합니다.
	bool Start();

	// 로그 시스템을 종료합니다.
	bool Stop();
	
	// 로그 시스템 종료요청로그인지 확인합니다.
	bool IsStopRequest(BGLog&);

	// queue에서 로그를 하나 꺼냅니다.
	BGLog& Pick();

	// queue에 로그를 넣습니다.
	void Push(BGLog&);

	// log를 기록합니다.
	void Write(BGLog&);

	// 로그 전용 스레드 함수 입니다.
	static void Run(BGLogManager*);



private:
	std::mutex m_queueLock;
	LogQueue m_queue;

	std::thread* m_pRunThread;
	
};

