#pragma once

class BG_ERROR {
public:
	enum class Common {
		BG_NONE,	// ����
		BG_ERROR,	// ����	
	};

	enum class Timer {
		BG_NONE,	// ����
		BG_ERROR,	// ����
	};

	static int ErrorCode(Common error) { return static_cast<int>(error); }
	static int ErrorCode(Timer error) { return static_cast<int>(error); }
};


