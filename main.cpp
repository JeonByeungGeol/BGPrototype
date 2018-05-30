// BGPrototype.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"


int main()
{
	if (!g_Config.Load()) {
		return -1;
	}
	
	

    return 0;
}