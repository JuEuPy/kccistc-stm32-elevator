#ifndef APP_INC_ELEVATORCONTROLLER_H_
#define APP_INC_ELEVATORCONTROLLER_H_

#include <stdint.h>
#include <stdbool.h>

//엘리베이터의 상태를 관리하는 ENUM
typedef enum {
    STATE_IDLE,         // 정지
    STATE_MOVING_UP,    // 상승중
    STATE_MOVING_DOWN,  // 하강중
    STATE_ARRIVED,      // 층 도착
    STATE_ERROR         // 잘못된 목표층, 모터/엔코더 타임아웃 등 이상 상황
} ElevatorState_t;

void elevatorControllerInit(void);
void elevatorControllerUpdate(void);
ElevatorState_t elevatorControllerGetState(void);

/* 현재 층 위치 (기본값 1층에서 시작) */
uint8_t elevatorControllerGetFloor(void);

/*
 * 큐에 새로 등록된 목표 층의 LED를 즉시 켠다
 */
void elevatorControllerLightFloorLed(uint8_t floor);

#endif /* APP_INC_ELEVATORCONTROLLER_H_ */
