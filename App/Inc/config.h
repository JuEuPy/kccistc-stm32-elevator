#ifndef APP_INC_CONFIG_H_
#define APP_INC_CONFIG_H_

/* 층수: 나중에 층이 늘어나면 이 값과 config.h의 GPIO 배열만 확장하면 됨 */
#define NUM_FLOORS          3U
#define FLOOR_MIN           1U
#define FLOOR_MAX           (FLOOR_MIN + NUM_FLOORS - 1U)

/* 버튼 디바운싱 시간 (ms) */
#define BUTTON_DEBOUNCE_MS  20U

/* 버튼/센서 스캔 주기 (ms), App_Run()에서 SysTick 기반으로 사용 */
#define APP_TICK_MS         1U

#endif /* APP_INC_CONFIG_H_ */
