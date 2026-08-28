#include "elevatorController.h"
#include "scheduler.h"
#include "motor.h"
#include "config.h"
#include "floorEncoder.h"
#include "buzzer.h"

#define elevatorControllerGetCurrentFloor() floorEncoderGetCurrentFloor()

static ElevatorState_t s_state;
static uint8_t s_current_floor = FLOOR_MIN; /* 현재 층 위치, 기본값 1층에서 시작 */

/* 지나가는 층마다 갱신: 현재 층에 해당하는 LED만 켜고 나머지는 끈다 */
static void elevatorControllerNotifyFloor(void)
{
    HAL_GPIO_WritePin(FLOOR_LED_1_GPIO_Port, FLOOR_LED_1_Pin, (s_current_floor == 1) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(FLOOR_LED_2_GPIO_Port, FLOOR_LED_2_Pin, (s_current_floor == 2) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(FLOOR_LED_3_GPIO_Port, FLOOR_LED_3_Pin, (s_current_floor == 3) ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

void elevatorControllerInit(void){
    floorEncoderInit();
    motorInit();
    // buzzerInit();
    elevatorControllerNotifyFloor(); 
    buzzerPlayFloorTone(s_current_floor); // 시작 층 알림음
}


void elevatorControllerUpdate(void){
    /* TODO */
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

    while (s_current_floor < target_floor) {
        int32_t start_pulse = floorEncoderGetPulseCount();
        // uint32_t start_tick = HAL_GetTick();

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
        elevatorControllerNotifyFloor();
    }

    while (s_current_floor > target_floor) {
        int32_t start_pulse = floorEncoderGetPulseCount();
        uint32_t start_tick = HAL_GetTick();

        s_state = STATE_MOVING_DOWN;
        motorDown();
        while ((floorEncoderGetPulseCount() - start_pulse) < (int32_t)FLOOR_15CM_PULSE) {
            if ((HAL_GetTick() - start_tick) > MOTOR_MOVE_TIMEOUT_MS) {
                motorStop();
                s_state = STATE_ERROR;
                return false;
            }
        }
        motorStop();

        s_current_floor--;
        elevatorControllerNotifyFloor();
    }

    s_state = STATE_ARRIVED;
    buzzerPlayFloorTone(s_current_floor); /* 최종 도착 층에서만 1회 울림 */
    return true;
}
