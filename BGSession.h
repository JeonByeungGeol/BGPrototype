#pragma once

/**
* <pre>
* ���� ��ü
* </pre>
*/

class BGSession
{
public:
	~BGSession();

	/** �� ������ ����ϴ� */
	static BGSession* Create(const int& index);




private:
	/** �ܺ� ���� ���� */
	explicit BGSession(const int& index);

	/** ���� �ε��� : ���� �ѹ� ���� �Ǹ�, �ٲ��� �ʽ��ϴ�.*/
	int m_Index;
};

