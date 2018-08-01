#pragma once

/** 설정 파일 이름 */
static const char* const BG_CONFIG_FILE_NAME = "bg_config.ini";

/** 세션 풀 크기 : 10001개 세션 풀 유지*/
static const int BG_SESSION_POOL_SIZE = 10001;




/** 타이머에서 동작할 스레드 갯수*/
static const int BG_TIMER_THREAD_NUM = 1;