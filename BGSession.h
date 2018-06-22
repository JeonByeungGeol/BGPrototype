#pragma once

#include "BGSessionEnum.h"

/**
* <pre>
* ���� ������
* </pre>
*/
struct BG_SESSION_DATA
{
	int sampleData1;

	std::string sampleData2;


	void Reset() {
		sampleData1 = 0;
		sampleData2.clear();
	}
};


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
	
	/** ���� ����*/
	ESessionState m_State;

	/** ������*/
	BG_SESSION_DATA m_Data;
};

