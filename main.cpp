// BGPrototype.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"


int main()
{
	if (!g_Config.Load()) {
		std::cout << "g_Config.Load() Failed!" << std::endl;
		return -1;
	}
	
	if (!g_LogManager.Start()) {
		std::cout << "g_LogManager.Start() Failed!" << std::endl;
		return -1;
	}

	while (1)
	{
		char c;
		std::cin >> c;
		if (c == 'e')
		{
			break;
		}
		Sleep(1000);
	}

	if (!g_LogManager.Stop()) {
		std::cout << "g_LogManager.Stop() Failed!" << std::endl;
		return -1;
	}

    return 0;
}