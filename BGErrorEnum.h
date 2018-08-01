#pragma once

class BG_ERROR {
public:
	enum class Common {
		BG_NONE,	// 정상
		BG_ERROR,	// 에러	
	};

	enum class Timer {
		BG_NONE,	// 정상
		BG_ERROR,	// 에러
	};

	static int ErrorCode(Common error) { return static_cast<int>(error); }
	static int ErrorCode(Timer error) { return static_cast<int>(error); }
};


