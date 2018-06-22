#pragma once

/**
* <pre>
* 세션 객체
* </pre>
*/

class BGSession
{
public:
	~BGSession();

	/** 새 새션을 만듭니다 */
	static BGSession* Create(const int& index);




private:
	/** 외부 생성 금지 */
	explicit BGSession(const int& index);

	/** 세션 인덱스 : 값은 한번 정의 되면, 바뀌지 않습니다.*/
	int m_Index;
};

