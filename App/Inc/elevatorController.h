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
 * target_floor까지 이동. 층 하나 이동 = FLOOR_15CM_PULSE만큼 펄스가 쌓일 때까지 모터 구동.
 * target_floor가 FLOOR_MIN~FLOOR_MAX 범위를 벗어나거나 이동 중 타임아웃(모터/엔코더 이상)이
 * 발생하면 모터를 멈추고 false를 반환한다. 정상적으로 도착하면 true를 반환한다.
 */
bool elevatorControllerMoveToFloor(uint8_t target_floor);

#endif /* APP_INC_ELEVATORCONTROLLER_H_ */
