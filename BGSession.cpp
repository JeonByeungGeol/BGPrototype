#include "stdafx.h"
#include "BGSession.h"


BGSession::BGSession()
{
}


BGSession::~BGSession()
{
}

BGSession * BGSession::Create(const int & index)
{
	return new BGSession(index);
}
