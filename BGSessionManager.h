#pragma once

#include "BGSession.h"

/**
* <pre>
* ������ ����
* </pre>
*/

class BGSessionManager
{
	using UnusedSessionQueue = std::queue<BGSession*>;

public:
	BGSessionManager();
	~BGSessionManager();

	/** ���Ǹ޴��� ����� �����մϴ�.*/
	bool Start();

	/** ���Ǹ޴��� ����� �����մϴ�.*/
	bool Stop();

private:
	/** ���� Ǯ */
	std::array<BGSession*, BG_SESSION_POOL_SIZE> m_SessionArray;

	/** ����� �� �ִ� ���� ť */
	UnusedSessionQueue m_UnusedSessionQueue;

	
};

