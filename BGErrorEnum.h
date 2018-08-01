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

		BG_INVALID_TIMER_EVENT_TYPE,	// 객체에 정의되지 않은 타이머 이벤트 타입
	};

	static int ErrorCode(Common error) { return static_cast<int>(error); }
	static int ErrorCode(Timer error) { return static_cast<int>(error); }
};


