#pragma once

#include "BGSession.h"

/**
* <pre>
* 세션을 관리
* </pre>
*/

class BGSessionManager
{
	using UnusedSessionQueue = std::queue<BGSession*>;

public:
	BGSessionManager();
	~BGSessionManager();

	/** 세션메니저 사용을 시작합니다.*/
	bool Start();

	/** 세션메니저 사용을 종료합니다.*/
	bool Stop();

private:
	/** 세션 풀 */
	std::array<BGSession*, BG_SESSION_POOL_SIZE> m_SessionArray;

	/** 사용할 수 있는 세션 큐 */
	UnusedSessionQueue m_UnusedSessionQueue;

	
};

