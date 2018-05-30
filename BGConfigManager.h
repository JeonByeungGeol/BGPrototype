#pragma once

/**
 * <pre>
 * 설정 값들을 관리
 * </pre>
*/

class BGConfigManager
{
	using ConfigMap = std::map<std::string, std::string>;

public:
	BGConfigManager();
	~BGConfigManager();

	// config.ini 파일을 읽습니다.
	bool Load();


	// config 값을 읽어 옵니다.
	int GetInt();
	std::string GetString();
	double GetDouble();

private:
	// config 값들을 저장합니다.
	ConfigMap m_ConfigMap;
};

