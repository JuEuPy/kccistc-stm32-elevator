#ifndef APP_INC_CONFIG_H_
#define APP_INC_CONFIG_H_

#include "main.h"

/* 층수: 나중에 층이 늘어나면 이 값과 config.h의 GPIO 배열 변경 */
#define NUM_FLOORS          3U
#define FLOOR_MIN           1U
#define FLOOR_MAX           (FLOOR_MIN + NUM_FLOORS - 1U)

/* 버튼 디바운싱 시간 (ms) */
#define BUTTON_DEBOUNCE_MS  20U

/* 버튼/센서 스캔 주기 (ms), appRun()에서 SysTick 기반으로 사용 */
#define APP_TICK_MS         1U

/*
 * =============================================== 모터 드라이버 (CubeMX 확정: MOT_IN3/IN4, main.h 참고)
 */

// 정회전 (상승)
#define MOTOR_IN1_GPIO_Port  MOT_IN3_GPIO_Port
#define MOTOR_IN1_Pin        MOT_IN3_Pin

// 역회전 (하강)
#define MOTOR_IN2_GPIO_Port  MOT_IN4_GPIO_Port
#define MOTOR_IN2_Pin        MOT_IN4_Pin

// 구동 인가 (pwm 으로 속도 제어)
 
#define MOTOR_PWM_TIM        htim4
#define MOTOR_PWM_CHANNEL    TIM_CHANNEL_3
#define MOTOR_PWM_ARR        999U   // Counter Period 

// 속도(%) 
#define MOTOR_SPEED_DEFAULT  60U
#define MOTOR_SPEED_MIN      60U
#define MOTOR_SPEED_MAX      100U

#endif /* APP_INC_CONFIG_H_ */
