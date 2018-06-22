#include "stdafx.h"
#include "BGSession.h"


BGSession::BGSession(const int& index)
	: m_Index(index)
{
}


BGSession::~BGSession()
{
}

BGSession * BGSession::Create(const int & index)
{
	return new BGSession(index);
}

bool BGSession::Reset()
{
	m_Data.Reset();
	
	return true;
}
