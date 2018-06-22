#include "stdafx.h"
#include "BGSessionManager.h"


BGSessionManager::BGSessionManager()
{
}


BGSessionManager::~BGSessionManager()
{
}

bool BGSessionManager::Start()
{
	for (int i = 1; i <= BG_SESSION_POOL_SIZE; i++)
	{
		BGSession * pSession{ nullptr };
		pSession = BGSession::Create(i);
		m_SessionArray[i] = pSession;
		m_UnusedSessionQueue.push(pSession);
	}

	BG_LOG_DEBUG("SessionManager Start Success");

	return true;
}

bool BGSessionManager::Stop()
{
	for (auto& session : m_SessionArray) {
		delete session;
		session = nullptr;
	}

	BG_LOG_DEBUG("SessionManager Stop Success");

	return true;
}
