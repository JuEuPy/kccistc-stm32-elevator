#include "elevatorController.h"
#include "scheduler.h"
#include "motor.h"
#include "config.h"
#include "floorEncoder.h"
#include "buzzer.h"
#include <stdio.h>

#define elevatorControllerGetCurrentFloor() floorEncoderGetCurrentFloor()

static ElevatorState_t s_state;
static uint8_t s_current_floor = FLOOR_MIN;   /* 현재 층 위치, 기본값 1층에서 시작 */
static uint8_t s_target_floor;                /* 지금 향하고 있는 목표 층 */
static int32_t s_step_start_pulse;            /* 현재 한 층 스텝을 시작한 시점의 펄스값 */
static uint32_t s_step_start_tick;            /* 현재 한 층 스텝을 시작한 시점의 tick */

/* floor에 해당하는 LED만 켜고 나머지는 끈다 (호출 시점의 목표/현재 층 어느 쪽으로도 쓸 수 있게 파라미터화) */
static void elevatorControllerSetFloorLed(uint8_t floor)
{
    HAL_GPIO_WritePin(FLOOR_LED_1_GPIO_Port, FLOOR_LED_1_Pin, (floor == 1) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(FLOOR_LED_2_GPIO_Port, FLOOR_LED_2_Pin, (floor == 2) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(FLOOR_LED_3_GPIO_Port, FLOOR_LED_3_Pin, (floor == 3) ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

void elevatorControllerInit(void){
    floorEncoderInit();
    motorInit();
    // buzzerInit();
    elevatorControllerSetFloorLed(s_current_floor);
    buzzerPlayFloorTone(s_current_floor); // 시작 층 알림음
}

ElevatorState_t elevatorControllerGetState(void){
    return s_state;
}

uint8_t elevatorControllerGetFloor(void){
    return s_current_floor;
}

bool elevatorControllerMoveToFloor(uint8_t target_floor){
    if (target_floor < FLOOR_MIN || target_floor > FLOOR_MAX) {
        s_state = STATE_ERROR;
        return false;
    }

    elevatorControllerSetFloorLed(target_floor); /* 버튼 누른 즉시 목표 층 LED 점등 */

    while (s_current_floor < target_floor) {
        int32_t start_pulse = floorEncoderGetPulseCount();
        // uint32_t start_tick = HAL_GetTick();

    if (target_floor > s_current_floor) {
        s_state = STATE_MOVING_UP;
        motorUp();
        while ((start_pulse - floorEncoderGetPulseCount()) < (int32_t)FLOOR_15CM_PULSE) {
            // if ((HAL_GetTick() - start_tick) > MOTOR_MOVE_TIMEOUT_MS) {
            //     motorStop();
            //     s_state = STATE_ERROR;
            //     return false;
            // }
        }
        motorStop();

        s_current_floor++;
        elevatorControllerSetFloorLed(s_current_floor); /* 지나가는 층 표시 */
    }

    while (s_current_floor > target_floor) {
        int32_t start_pulse = floorEncoderGetPulseCount();
        uint32_t start_tick = HAL_GetTick();

        s_state = STATE_MOVING_DOWN;
        motorDown();
    }
}

/*
 * 논블로킹 상태 머신. appRun()에서 매 tick마다 호출된다.
 */
void elevatorControllerUpdate(void){
    
    switch (s_state) {
    case STATE_IDLE:
        if (!schedulerHasPendingRequests()) {
            break;
        }

        s_target_floor = schedulerGetNextTarget(s_current_floor);
        printf("IDLE: 목표층t=%u (현재=%u)\r\n", s_target_floor, s_current_floor);
        elevatorControllerLightFloorLed(s_target_floor);

        if (s_target_floor == s_current_floor) {
            s_state = STATE_ARRIVED; /* 이미 그 층이면 이동 없이 바로 도착 처리 */
        } else {
            elevatorControllerStartStep(s_target_floor);
        }
        break;

    case STATE_MOVING_UP:
        {
            int32_t delta = s_step_start_pulse - floorEncoderGetPulseCount();
            static uint32_t s_debug_tick = 0;

            if (delta >= (int32_t)FLOOR_15CM_PULSE) {
                motorStop();
                s_current_floor++;

                if (s_current_floor == s_target_floor) {
                    s_state = STATE_ARRIVED;
                } else {
                    elevatorControllerStartStep(s_target_floor);
                }
            } else if ((HAL_GetTick() - s_debug_tick) > 300U) {
                /* TODO(임시 디버그): 펄스가 실제로 움직이는지 확인용. 원인 파악되면 지울 것 */
                s_debug_tick = HAL_GetTick();
                printf("MOVING_UP debug: delta=%ld / target=%u\r\n", (long)delta, (unsigned)FLOOR_15CM_PULSE);
            }
        }
        /* TODO: MOTOR_MOVE_TIMEOUT_MS 기반 타임아웃 (현재 비활성화 상태) */
        break;

        s_current_floor--;
        elevatorControllerSetFloorLed(s_current_floor); /* 지나가는 층 표시 */
    }
}
