#include "door.h"
#include "config.h"
#include "tim.h"

static DoorState_t s_door_state = DOOR_STATE_CLOSED;

/* 서보 펄스폭(CCR) 설정 */
static void doorSetPulse(uint32_t ccr){
    __HAL_TIM_SET_COMPARE(&DOOR_PWM_TIM, DOOR_PWM_CHANNEL, ccr);
}

// 도어 서보 초기화. 시작 시 닫힘 위치로 세팅
void doorInit(void){
    HAL_TIM_PWM_Start(&DOOR_PWM_TIM, DOOR_PWM_CHANNEL);
    doorSetPulse(DOOR_CLOSED_CCR);
    s_door_state = DOOR_STATE_CLOSED;
}

// 문 열기
void doorOpen(void){
    doorSetPulse(DOOR_OPEN_CCR);
    /* 서보가 목표 위치까지 움직이는 시간을 단순 대기로 확보. 실측 후 DOOR_MOVE_TIME_MS 조정 */
    HAL_Delay(DOOR_MOVE_TIME_MS);
    s_door_state = DOOR_STATE_OPEN;
}

// 문 닫기
void doorClose(void){
    doorSetPulse(DOOR_CLOSED_CCR);
    HAL_Delay(DOOR_MOVE_TIME_MS);
    s_door_state = DOOR_STATE_CLOSED;
}

// 현재 문 상태 조회
DoorState_t doorGetState(void){
    return s_door_state;
}
