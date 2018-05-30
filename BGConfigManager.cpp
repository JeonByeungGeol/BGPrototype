#include "stdafx.h"
#include "BGConfigManager.h"


BGConfigManager::BGConfigManager()
{
}


BGConfigManager::~BGConfigManager()
{
	m_ConfigMap.clear();
}

bool BGConfigManager::Load()
{
	return false;
}

int BGConfigManager::GetInt()
{
	return 0;
}

std::string BGConfigManager::GetString()
{
	return std::string();
}

double BGConfigManager::GetDouble()
{
	return 0.0;
}


