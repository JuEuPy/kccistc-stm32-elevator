#ifndef APP_INC_ELEVATORCONTROLLER_H_
#define APP_INC_ELEVATORCONTROLLER_H_

#include <stdint.h>

//엘리베이터의 상태를 관리하는 ENUM
typedef enum {
    STATE_IDLE,         // 정지
    STATE_MOVING_UP,    // 상승중
    STATE_MOVING_DOWN,  // 하강중
    STATE_ARRIVED       // 층 도착
} ElevatorState_t;

void elevatorControllerInit(void);
void elevatorControllerUpdate(void);
ElevatorState_t elevatorControllerGetState(void);

/* 현재 층 위치 (기본값 1층에서 시작) */
uint8_t elevatorControllerGetFloor(void);

/* target_floor까지 이동. 층 하나 이동 = FLOOR_15CM_PULSE만큼 펄스가 쌓일 때까지 모터 구동 */
void elevatorControllerMoveToFloor(uint8_t target_floor);

#endif /* APP_INC_ELEVATORCONTROLLER_H_ */
