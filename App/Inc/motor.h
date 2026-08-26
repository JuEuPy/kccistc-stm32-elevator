#ifndef APP_INC_MOTOR_H_
#define APP_INC_MOTOR_H_

#include <stdint.h>

// 모터의 동작 상태를 관리하는 ENUM
typedef enum {
    MOTOR_STATE_STOPPED,
    MOTOR_STATE_UP,
    MOTOR_STATE_DOWN
} MotorState_t;

void motorInit(void);
void motorUp(void);
void motorDown(void);
void motorStop(void);

MotorState_t motorGetState(void);

/* 속도 설정 */
void motorSetSpeed(uint8_t percent);

#endif /* APP_INC_MOTOR_H_ */
