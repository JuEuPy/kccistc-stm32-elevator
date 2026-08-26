#include "motor.h"
#include "config.h"
#include "tim.h"

static uint8_t s_speed_percent = MOTOR_SPEED_DEFAULT;
static MotorState_t s_motor_state = MOTOR_STATE_STOPPED;

// 모터 초기화 함수
void motorInit(void){
    HAL_GPIO_WritePin(MOTOR_IN1_GPIO_Port, MOTOR_IN1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(MOTOR_IN2_GPIO_Port, MOTOR_IN2_Pin, GPIO_PIN_RESET);
    HAL_TIM_PWM_Start(&MOTOR_PWM_TIM, MOTOR_PWM_CHANNEL);
    motorSetSpeed(s_speed_percent);
    s_motor_state = MOTOR_STATE_STOPPED;
}

// 모터 상승
void motorUp(void){
    /* 방향 전환은 항상 IN1/IN2를 끈 상태에서 하고 마지막에 켠다 */
    HAL_GPIO_WritePin(MOTOR_IN1_GPIO_Port, MOTOR_IN1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(MOTOR_IN2_GPIO_Port, MOTOR_IN2_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(MOTOR_IN1_GPIO_Port, MOTOR_IN1_Pin, GPIO_PIN_SET);
    s_motor_state = MOTOR_STATE_UP;
}

// 모터 하강
void motorDown(void){
    HAL_GPIO_WritePin(MOTOR_IN1_GPIO_Port, MOTOR_IN1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(MOTOR_IN2_GPIO_Port, MOTOR_IN2_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(MOTOR_IN2_GPIO_Port, MOTOR_IN2_Pin, GPIO_PIN_SET);
    s_motor_state = MOTOR_STATE_DOWN;
}

// 모터 정지
void motorStop(void){
    HAL_GPIO_WritePin(MOTOR_IN1_GPIO_Port, MOTOR_IN1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(MOTOR_IN2_GPIO_Port, MOTOR_IN2_Pin, GPIO_PIN_RESET);
    s_motor_state = MOTOR_STATE_STOPPED;
}

// 현재 모터 상태 조회
MotorState_t motorGetState(void){
    return s_motor_state;
}

// 속도 설정 (0~100%)
void motorSetSpeed(uint8_t percent){
    if (percent < MOTOR_SPEED_MIN) {
        percent = MOTOR_SPEED_MIN;
    } else if (percent > MOTOR_SPEED_MAX) {
        percent = MOTOR_SPEED_MAX;
    }

    s_speed_percent = percent;
    __HAL_TIM_SET_COMPARE(&MOTOR_PWM_TIM, MOTOR_PWM_CHANNEL, (MOTOR_PWM_ARR + 1U) * s_speed_percent / 100U);
}
