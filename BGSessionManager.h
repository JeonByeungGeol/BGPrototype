#pragma once

#include "BGSession.h"

/**
* <pre>
* ������ ����
* </pre>
*/

class BGSessionManager
{
public:
	BGSessionManager();
	~BGSessionManager();

	/** ���Ǹ޴��� ����� �����մϴ�.*/
	bool Start();

	/** ���Ǹ޴��� ����� �����մϴ�.*/
	bool Stop();
};

