#include "stdafx.h"
#include "BGUtil.h"


std::string BGUtil::ToString(std::wstring wstr)
{
	std::string str;
	str.assign(wstr.begin(), wstr.end());

	return str;
}

std::wstring BGUtil::ToWString(std::string str)
{
	std::wstring wstr;
	wstr.assign(str.begin(), str.end());

	return wstr;
}
