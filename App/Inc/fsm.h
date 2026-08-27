#ifndef APP_INC_FSM_H_
#define APP_INC_FSM_H_

//엘리베이터의 상태를 관리하는 ENUM
typedef enum {
    STATE_IDLE,         // 정지
    STATE_MOVING_UP,    // 상승중
    STATE_MOVING_DOWN,  // 하강중
    STATE_ARRIVED       // 층 도착
} ElevatorState_t;

void fsmInit(void);
void fsmUpdate(void);
ElevatorState_t fsmGetState(void);

#endif /* APP_INC_FSM_H_ */