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
 * 모터 드라이버 (CubeMX 확정: MOT_IN3/IN4, main.h 참고)
 */

// 정회전 (상승)
#define MOTOR_IN1_GPIO_Port  MOT_IN3_GPIO_Port
#define MOTOR_IN1_Pin        MOT_IN3_Pin

// 역회전 (하강)
#define MOTOR_IN2_GPIO_Port  MOT_IN4_GPIO_Port
#define MOTOR_IN2_Pin        MOT_IN4_Pin

/*
 * 구동 인가 (pwm 으로 속도 제어)
 * CubeMX에서 TIM4를 PWM 모드로 생성 필요
 */
#define MOTOR_PWM_TIM        htim4
#define MOTOR_PWM_CHANNEL    TIM_CHANNEL_3
#define MOTOR_PWM_ARR        999U   // Counter Period 

/* 속도(%) 기본값/범위: 나중에 실측 후 조정 */
#define MOTOR_SPEED_DEFAULT  75U
#define MOTOR_SPEED_MIN      75U
#define MOTOR_SPEED_MAX      100U

/*
 * 층 위치 감지. 
 * 구조물 총 높이 45cm / 3개 층 = 층당 15cm
 */
#define STRUCTURE_HEIGHT_CM  45U
#define FLOOR_HEIGHT_CM      (STRUCTURE_HEIGHT_CM / NUM_FLOORS)  

/*
 * TRIG: 출력, ECHO: 입력 (CubeMX 확정: SONIC_TRIG/ECHO, main.h 참고)
 */
#define SENSOR_TRIG_GPIO_Port SONIC_TRIG_GPIO_Port
#define SENSOR_TRIG_Pin       SONIC_TRIG_Pin
#define SENSOR_ECHO_GPIO_Port SONIC_ECHO_GPIO_Port
#define SENSOR_ECHO_Pin       SONIC_ECHO_Pin

/* 응답 타임아웃 */
#define SENSOR_ECHO_START_TIMEOUT_US 5000U  
#define SENSOR_ECHO_END_TIMEOUT_US   30000U 

/* 벗어나면 측정 실패로 처리 */
#define SENSOR_DISTANCE_MIN_CM       2U
#define SENSOR_DISTANCE_MAX_CM       400U

/*
 * 층 감지 방식 전환
 * elevatorController.c가 이 값을 보고 floorSensor(초음파) / encoder(모터 축 엔코더) 중 하나만 쓴다.
 */
#define FLOOR_SENSE_ULTRASONIC  0   //수정안함
#define FLOOR_SENSE_ENCODER     1   //수정안함
#define FLOOR_SENSE_METHOD      FLOOR_SENSE_ENCODER //필요시 수정

/*
 * 모터(SE-DM185) 축 내장 엔코더 기반 층 감지. 
 */
#define ENCODER_TIM          htim3
#define FLOOR_15CM_PULSE     10000U // 임시값

/* 한 층 이동 중 이 시간(ms) 안에 도착 펄스가 안 쌓이면 모터/엔코더 이상으로 보고 중단 */
#define MOTOR_MOVE_TIMEOUT_MS 5000U

/*
 * 엔코더 카운터가 이 값을 넘으면(절대값 기준) 0으로 재설정한다.
 * TIM3는 16비트라 32767을 넘으면 음수로 언더플로우(wraparound)하므로 그 전에 리셋.
 * NUM_FLOORS=3 기준 최대 이동거리(2 * FLOOR_15CM_PULSE)보다 넉넉히 커야 함.
 */
#define ENCODER_PULSE_RESET_THRESHOLD  30000

/*
 * 외문(슬라이딩) 서보모터 1개 
 * 임시값. 실측 후 조정 필요.
 */
#define DOOR_PWM_TIM         htim2
#define DOOR_PWM_CHANNEL     TIM_CHANNEL_2
#define DOOR_PWM_ARR         1999U   // Counter Period

/* 닫힘/열림 위치에 대응하는 서보 펄스폭(CCR). 임시값(1.0ms/1.5ms 부근) */
#define DOOR_CLOSED_CCR      100U
#define DOOR_OPEN_CCR        150U

/* 문이 열리고/닫히는 데 걸리는 대략적 시간(ms). 실측 후 조정 */
#define DOOR_MOVE_TIME_MS    500U

/*
 * 층 위치 표시 LED 
 * 1층=빨강(PB6), 2층=노랑(PA7), 3층=초록(PA6)
 */
#define FLOOR_LED_1_GPIO_Port LED_1_GPIO_Port
#define FLOOR_LED_1_Pin       LED_1_Pin
#define FLOOR_LED_2_GPIO_Port LED_2_GPIO_Port
#define FLOOR_LED_2_Pin       LED_2_Pin
#define FLOOR_LED_3_GPIO_Port LED_3_GPIO_Port
#define FLOOR_LED_3_Pin       LED_3_Pin

#endif /* APP_INC_CONFIG_H_ */