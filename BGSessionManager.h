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

	/** ��ü ����� �����մϴ�.*/
	bool Start();

	/** ��ü ����� �����մϴ�.*/
	bool Stop();
	
	/** ������ ���ɴϴ�.*/
	BGSession* GetSession(int index);

	/** ���� ����� ������ ���ɴϴ�.*/
	BGSession* GetSessionNew();

	/** ������� �ʴ� ������ �ݳ��մϴ�.*/
	bool ReturnSession(BGSession*);
	
	/** ����� ������ ����մϴ�.*/
	bool AddSession(BGSession*);

	/** ������� �ʴ� ������ �ݳ��մϴ�.*/
	bool RemoveSession(BGSession*);
	
private:
	/** ���� Ǯ */
	std::array<BGSession*, BG_SESSION_POOL_SIZE> m_SessionArray;

	/** ����� �� �ִ� ���� ť */
	UnusedSessionQueue m_UnusedSessionQueue;

	/** UnusedSessionQueue�� ����ȭ ��Ű�� ���ؽ�*/
	std::mutex m_SessionQueueMutex;
	
};

