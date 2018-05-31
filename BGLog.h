#pragma once

#include "BGLogEnum.h"

class BGLog
{
public:
	BGLog() : m_logLevel(ELogLevel::NONE) {};
	~BGLog();


public:
	bool Valid() { return m_logLevel != ELogLevel::NONE; }

private:
	ELogLevel m_logLevel;
	std::string m_content;
};

