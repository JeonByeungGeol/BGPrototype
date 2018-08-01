#pragma once
class BGUtil
{
public:
	BGUtil() = default;
	~BGUtil() = default;

public:
	/** wstr -> str */
	static std::string ToString(std::wstring);

	/** str -> wstr */
	static std::wstring ToWString(std::string);
};

