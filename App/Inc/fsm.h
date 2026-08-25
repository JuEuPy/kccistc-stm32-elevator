#ifndef APP_INC_FSM_H_
#define APP_INC_FSM_H_

typedef enum {
    STATE_IDLE,
    STATE_MOVING_UP,
    STATE_MOVING_DOWN,
    STATE_ARRIVED
} ElevatorState_t;

void FSM_Init(void);
void FSM_Update(void);
ElevatorState_t FSM_GetState(void);

#endif /* APP_INC_FSM_H_ */
