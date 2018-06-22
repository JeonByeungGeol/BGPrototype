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
	for (int i = 0; i < BG_SESSION_POOL_SIZE; i++)
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

BGSession * BGSessionManager::GetSession(int index)
{
	if (index >= BG_SESSION_POOL_SIZE) {
		BG_LOG_ERROR("index >= BG_SESSION_POOL_SIZE");
		return nullptr;
	}

	return m_SessionArray[index];
}

/**
 * ���� Ǯ���� ��밡���� ������ �ϳ� ���ɴϴ�.
 * �̶�, ��밡���� ���� Ǯ���� �����ϱ� ������ ���Ŀ� �ݵ�� AddSession�� ȣ���ؾ� �մϴ�.
 * ���� ������� �������� ReturnSession�� ȣ���ؼ� ������ �ٽ� �ݳ��ؾ� �մϴ�.
*/
BGSession * BGSessionManager::GetSessionNew()
{	
	m_SessionQueueMutex.lock();

	if (m_UnusedSessionQueue.empty()) {
		BG_LOG_WARNING("Session Pool is Max");
		m_SessionQueueMutex.unlock();
		return nullptr;
	}

	auto pSession = m_UnusedSessionQueue.front();
	m_UnusedSessionQueue.pop();
	
	m_SessionQueueMutex.unlock();

	return pSession;
}

/** 
 * ������ ��밡���� ���� Ǯ�� �ֽ��ϴ�.
*/
bool BGSessionManager::ReturnSession(BGSession * pSession)
{
	if (pSession == nullptr) {
		BG_LOG_ERROR("pSession is nullptr");
		return false;
	}

	m_SessionQueueMutex.lock();
	m_UnusedSessionQueue.push(pSession);
	m_SessionQueueMutex.unlock();

	return true;
}

bool BGSessionManager::AddSession(BGSession * pSession)
{
	if (pSession == nullptr) {
		BG_LOG_ERROR("pSession is nullptr");
		return false;
	}

	return true;
}

bool BGSessionManager::RemoveSession(BGSession * pSession)
{
	if (pSession == nullptr) {
		BG_LOG_ERROR("pSession is nullptr");
		return false;
	}

	// ������ �ݳ��ϱ� ������ �������� �ʱ�ȭ
	pSession->Reset();
	
	// ���� �ݳ�
	if (false == ReturnSession(pSession)) {
		BG_LOG_ERROR("ReturnSession is fail");
		return false;
	}

	return true;
}
