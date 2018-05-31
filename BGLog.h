#pragma once

#include "BGLogEnum.h"

class BGLog
{
public:
	BGLog() : m_logLevel(ELogLevel::NONE) {};
	BGLog(ELogLevel level, std::string content)
		: m_logLevel(level), m_content(content)
	{}
	~BGLog();


public:
	bool Valid() { return m_logLevel != ELogLevel::NONE; }
	bool IsStopRequest() { if (m_logLevel == ELogLevel::INFO && m_content == "STOP") return true; return false; }
	ELogLevel GetLevel() { return m_logLevel; }

private:
	ELogLevel m_logLevel;
	std::string m_content;
};

