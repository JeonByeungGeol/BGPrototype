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

	/** ������ �ʱ�ȭ ��ŵ�ϴ�.*/
	bool Reset();



private:
	/** �ܺ� ���� ���� */
	explicit BGSession(const int& index);

	/** ���� �ε��� : ���� �ѹ� ���� �Ǹ�, �ٲ��� �ʽ��ϴ�.*/
	int m_Index;
};

