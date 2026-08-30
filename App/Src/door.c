#include "door.h"
#include "config.h"
#include "tim.h"

static DoorState_t s_state = DOOR_CLOSED;
static uint32_t s_state_start_tick;

void doorInit(void){
    HAL_TIM_PWM_Start(&DOOR_PWM_TIM, DOOR_PWM_CHANNEL);
    doorSetServoAngle(0.0f);
    s_state = DOOR_CLOSED;
}

void doorSetServoAngle(float angle){
    uint32_t pulse;

    if (angle < 0.0f) {
        angle = 0.0f;
    }
    if (angle > 180.0f) {
        angle = 180.0f;
    }

    pulse = 55U + (uint32_t)((angle * 200.0f) / 180.0f);
    __HAL_TIM_SET_COMPARE(&DOOR_PWM_TIM, DOOR_PWM_CHANNEL, pulse);
}

// 문열기
void doorOpen(void){
    if (s_state != DOOR_CLOSED) {
        return;
    }
    doorSetServoAngle(180.0f);
    s_state_start_tick = HAL_GetTick();
    s_state = DOOR_OPENING;
}

// 상태에 따라 문 열고 닫기
void doorUpdate(void){
    switch (s_state) {
    case DOOR_OPENING:
        if ((HAL_GetTick() - s_state_start_tick) >= DOOR_MOVE_TIME_MS) {
            s_state_start_tick = HAL_GetTick();
            s_state = DOOR_OPEN;
        }
        break;

    case DOOR_OPEN:
        if ((HAL_GetTick() - s_state_start_tick) >= DOOR_OPEN_HOLD_MS) {
            doorSetServoAngle(0.0f);
            s_state_start_tick = HAL_GetTick();
            s_state = DOOR_CLOSING;
        }
        break;

    case DOOR_CLOSING:
        if ((HAL_GetTick() - s_state_start_tick) >= DOOR_MOVE_TIME_MS) {
            s_state = DOOR_CLOSED;
        }
        break;

    case DOOR_CLOSED:
    default:
        break;
    }
}

DoorState_t doorGetState(void){
    return s_state;
}
