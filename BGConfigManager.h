#pragma once

/**
 * <pre>
 * ���� ������ ����
 * </pre>
*/

class BGConfigManager
{
	using ConfigMap = std::map<std::string, std::string>;

public:
	BGConfigManager();
	~BGConfigManager();

	// config.ini ������ �н��ϴ�.
	bool Load();


	// config ���� �о� �ɴϴ�.
	int GetInt();
	std::string GetString();
	double GetDouble();

private:
	// config ������ �����մϴ�.
	ConfigMap m_ConfigMap;
};

