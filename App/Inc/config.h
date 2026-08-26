#ifndef APP_INC_CONFIG_H_
#define APP_INC_CONFIG_H_

#include "main.h"

/* 층수: 나중에 층이 늘어나면 이 값과 config.h의 GPIO 배열 변경 */
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

/*
 * 모터 드라이버
 * TODO: 하드웨어 핀 배치 확정 전까지의 임시값. 확정되면 이 값들 갱신할 것
 */

// 정회전 (상승)
#define MOTOR_IN1_GPIO_Port  GPIOB   
#define MOTOR_IN1_Pin        GPIO_PIN_0

// 역회전 (하강)
#define MOTOR_IN2_GPIO_Port  GPIOB   /* 역회전(하강) 방향 제어 */
#define MOTOR_IN2_Pin        GPIO_PIN_1

// 구동 인가
#define MOTOR_ENA_GPIO_Port  GPIOB   
#define MOTOR_ENA_Pin        GPIO_PIN_2

// 속도(%) 
#define MOTOR_SPEED_DEFAULT  60U
#define MOTOR_SPEED_MIN      0U
#define MOTOR_SPEED_MAX      100U

#endif /* APP_INC_CONFIG_H_ */
