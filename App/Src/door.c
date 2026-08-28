#include "door.h"
#include "config.h"
#include "tim.h"

void doorInit(void){
    HAL_TIM_PWM_Start(&DOOR_PWM_TIM, DOOR_PWM_CHANNEL);
}

void doorSetServoAngle(float angle){
    uint32_t pulse;

    if (angle < 0.0f) {
        angle = 0.0f;
    }
    if (angle > 180.0f) {
        angle = 180.0f;
    }

    pulse = 50U + (uint32_t)((angle * 200.0f) / 180.0f);
    __HAL_TIM_SET_COMPARE(&DOOR_PWM_TIM, DOOR_PWM_CHANNEL, pulse);
}
