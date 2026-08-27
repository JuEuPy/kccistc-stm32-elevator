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
 * 모터 드라이버
 * TODO: 하드웨어 핀 배치 확정 전까지의 임시값. 확정되면 이 값들 갱신할 것
 */

// 정회전 (상승)
#define MOTOR_IN1_GPIO_Port  GPIOB   
#define MOTOR_IN1_Pin        GPIO_PIN_0

// 역회전 (하강)
#define MOTOR_IN2_GPIO_Port  GPIOB   /* 역회전(하강) 방향 제어 */
#define MOTOR_IN2_Pin        GPIO_PIN_1

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
 * 실측 거리(cm)가 몇 번째 15cm 구간에 속하는지로 현재 층을 판단
 */
#define STRUCTURE_HEIGHT_CM  45U
#define FLOOR_HEIGHT_CM      (STRUCTURE_HEIGHT_CM / NUM_FLOORS)  

/*
 * TRIG: 출력, ECHO: 입력
 */
#define SENSOR_TRIG_GPIO_Port GPIOA
#define SENSOR_TRIG_Pin       GPIO_PIN_8
#define SENSOR_ECHO_GPIO_Port GPIOA
#define SENSOR_ECHO_Pin       GPIO_PIN_9

/* 응답 타임아웃 */
#define SENSOR_ECHO_START_TIMEOUT_US 5000U  
#define SENSOR_ECHO_END_TIMEOUT_US   30000U 

/* 벗어나면 측정 실패로 처리 */
#define SENSOR_DISTANCE_MIN_CM       2U
#define SENSOR_DISTANCE_MAX_CM       400U

/*
 * 층 감지 방식 전환
 * fsm.c가 이 값을 보고 floorSensor(초음파) / encoder(모터 축 엔코더) 중 하나만 쓴다.
 */
#define FLOOR_SENSE_ULTRASONIC  0   //수정안함
#define FLOOR_SENSE_ENCODER     1   //수정안함
#define FLOOR_SENSE_METHOD      FLOOR_SENSE_ENCODER //필요시 수정

/*
 * 모터(SE-DM185) 축 내장 엔코더 기반 층 감지.
 * CubeMX에서 TIMx를 Encoder Mode(Combined Channels)로 설정 필요.
 */
#define ENCODER_TIM          htim3
#define FLOOR_15CM_PULSE     1000U // 임시값 

#endif /* APP_INC_CONFIG_H_ */