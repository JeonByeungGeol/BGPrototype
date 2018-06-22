#pragma once

#include "BGSession.h"

/**
* <pre>
* 세션을 관리
* </pre>
*/

class BGSessionManager
{
public:
	BGSessionManager();
	~BGSessionManager();

	/** 세션메니저 사용을 시작합니다.*/
	bool Start();

	/** 세션메니저 사용을 종료합니다.*/
	bool Stop();
};

