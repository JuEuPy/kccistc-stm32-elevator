#ifndef APP_INC_CONFIG_H_
#define APP_INC_CONFIG_H_

/* 층수: 나중에 층이 늘어나면 이 값과 config.h의 GPIO 배열만 확장하면 됨 */
#define NUM_FLOORS          3U
#define FLOOR_MIN           1U
#define FLOOR_MAX           (FLOOR_MIN + NUM_FLOORS - 1U)

/* 버튼 디바운싱 시간 (ms) */
#define BUTTON_DEBOUNCE_MS  20U

/*
 * =============================================== 버튼 
 * 내부 조작과 각 층 외부 호출 버튼을 역할별로 구분해서 사용
 */
#define BTN_1_GPIO_Port BTN_1_GPIO_Port
#define BTN_1_Pin       BTN_1_Pin
#define BTN_2_GPIO_Port BTN_2_GPIO_Port
#define BTN_2_Pin       BTN_2_Pin
#define BTN_3_GPIO_Port BTN_3_GPIO_Port
#define BTN_3_Pin       BTN_3_Pin

#define HALL_BTN_1_GPIO_Port  BTN_HALL_1_GPIO_Port
#define HALL_BTN_1_Pin        BTN_HALL_1_Pin
#define HALL_BTN_2_GPIO_Port  BTN_HALL_2_GPIO_Port
#define HALL_BTN_2_Pin        BTN_HALL_2_Pin
#define HALL_BTN_3_GPIO_Port  BTN_HALL_3_GPIO_Port
#define HALL_BTN_3_Pin        BTN_HALL_3_Pin

/* 카 버튼 + 홀 버튼 합친 총 개수 (button.c 배열 크기) */
#define BTN_COUNT             (NUM_FLOORS * 2U)

/* 층 호출 큐에 동시에 저장 가능한 최대 요청 수 */
#define SCHEDULER_QUEUE_CAPACITY 2U

/* 버튼/센서 스캔 주기 (ms), appRun()에서 SysTick 기반으로 사용 */
#define APP_TICK_MS         1U

#endif /* APP_INC_CONFIG_H_ */
