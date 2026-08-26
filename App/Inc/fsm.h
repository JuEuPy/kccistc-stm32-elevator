#ifndef APP_INC_FSM_H_
#define APP_INC_FSM_H_

typedef enum {
    STATE_IDLE,
    STATE_MOVING_UP,
    STATE_MOVING_DOWN,
    STATE_ARRIVED
} ElevatorState_t;

void fsmInit(void);
void fsmUpdate(void);
ElevatorState_t fsmGetState(void);

#endif /* APP_INC_FSM_H_ */
