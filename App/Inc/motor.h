#ifndef APP_INC_MOTOR_H_
#define APP_INC_MOTOR_H_

#include <stdint.h>

void motorInit(void);
void motorUp(void);
void motorDown(void);
void motorStop(void);

/* 속도 설정 */
void motorSetSpeed(uint8_t percent);

#endif /* APP_INC_MOTOR_H_ */
