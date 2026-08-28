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
static uint8_t s_departure_floor;             /* 이번 이동을 시작한 출발 층 (도착 후 LED 정리용) */
static int32_t s_step_start_pulse;            /* 현재 한 층 스텝을 시작한 시점의 펄스값 */
static uint32_t s_step_start_tick;            /* 현재 한 층 스텝을 시작한 시점의 tick */

/* floor에 해당하는 LED만 켜고 나머지는 끈다 */
static void elevatorControllerSetFloorLed(uint8_t floor){
    HAL_GPIO_WritePin(FLOOR_LED_1_GPIO_Port, FLOOR_LED_1_Pin, (floor == 1) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(FLOOR_LED_2_GPIO_Port, FLOOR_LED_2_Pin, (floor == 2) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(FLOOR_LED_3_GPIO_Port, FLOOR_LED_3_Pin, (floor == 3) ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

void elevatorControllerLightFloorLed(uint8_t floor){
    switch (floor) {
        case 1: HAL_GPIO_WritePin(FLOOR_LED_1_GPIO_Port, FLOOR_LED_1_Pin, GPIO_PIN_SET); 
            break;
        case 2: HAL_GPIO_WritePin(FLOOR_LED_2_GPIO_Port, FLOOR_LED_2_Pin, GPIO_PIN_SET); 
            break;
        case 3: HAL_GPIO_WritePin(FLOOR_LED_3_GPIO_Port, FLOOR_LED_3_Pin, GPIO_PIN_SET); 
            break;
        default: 
            break;
    }
}

/* elevatorControllerLightFloorLed()와 대칭: 그 층 LED만 끈다 */
static void elevatorControllerClearFloorLed(uint8_t floor){
    switch (floor) {
        case 1: HAL_GPIO_WritePin(FLOOR_LED_1_GPIO_Port, FLOOR_LED_1_Pin, GPIO_PIN_RESET); 
            break;
        case 2: HAL_GPIO_WritePin(FLOOR_LED_2_GPIO_Port, FLOOR_LED_2_Pin, GPIO_PIN_RESET); 
            break;
        case 3: HAL_GPIO_WritePin(FLOOR_LED_3_GPIO_Port, FLOOR_LED_3_Pin, GPIO_PIN_RESET); 
            break;
        default: 
            break;
    }
}

void elevatorControllerInit(void){
    floorEncoderInit();
    motorInit();
    // buzzerInit();
    s_state = STATE_IDLE;
    elevatorControllerSetFloorLed(s_current_floor);
    buzzerPlayFloorTone(s_current_floor); // 시작 층 알림음
}

// 엘리베이터 상태 반환
ElevatorState_t elevatorControllerGetState(void){
    return s_state;
}

// 층 반환
uint8_t elevatorControllerGetFloor(void){
    return s_current_floor;
}

/* target_floor 방향으로 한 스텝(한 층) 이동을 시작한다 */
static void elevatorControllerStartStep(uint8_t target_floor)
{
    s_step_start_pulse = floorEncoderGetPulseCount();
    s_step_start_tick = HAL_GetTick();

    if (target_floor > s_current_floor) {
        s_state = STATE_MOVING_UP;
        motorUp();
    } else {
        s_state = STATE_MOVING_DOWN;
        motorDown();
    }
}

/*
 * 층 이동/도착/에러 처리 등 엘리베이터 상태 관리
 * 논블로킹 상태 머신. appRun()에서 매 tick마다 호출된다.
 */
void elevatorControllerUpdate(void){
    
    switch (s_state) {
    case STATE_IDLE:
        if (!schedulerHasPendingRequests()) {
            break;
        }

        s_target_floor = schedulerGetNextTarget(s_current_floor); 
        s_departure_floor = s_current_floor;
        elevatorControllerClearFloorLed(s_departure_floor);
        elevatorControllerLightFloorLed(s_target_floor);

        if (s_target_floor == s_current_floor) {
            s_state = STATE_ARRIVED; /* 이미 그 층이면 이동 없이 바로 도착 처리 */
        } else {
            elevatorControllerStartStep(s_target_floor);
        }
        break;

    case STATE_MOVING_UP:
        {
            int32_t pulse = floorEncoderGetPulseCount();                // pulse
            int32_t raw_delta = pulse - s_step_start_pulse;         
            int32_t delta = (raw_delta < 0) ? -raw_delta : raw_delta;   // 이번 스텝에서 이동량
            static uint32_t s_debug_tick = 0;

            if (delta >= (int32_t)FLOOR_15CM_PULSE) {
                motorStop();
                s_current_floor++;

                if (s_current_floor == s_target_floor) {
                    s_state = STATE_ARRIVED;
                } else {
                    elevatorControllerStartStep(s_target_floor);
                }
            } else if ((HAL_GetTick() - s_step_start_tick) > MOTOR_MOVE_TIMEOUT_MS) {
                motorStop();
                s_state = STATE_ERROR;
            } else if ((HAL_GetTick() - s_debug_tick) > 300U) {
                /* TODO(임시 디버그): 펄스가 실제로 움직이는지 확인용. 원인 파악되면 지울 것 */
                s_debug_tick = HAL_GetTick();
                printf("MOVING_UP debug: pulse=%ld delta=%ld / target=%u\r\n", (long)pulse, (long)delta, (unsigned)FLOOR_15CM_PULSE);
            }
        }
        break;

    case STATE_MOVING_DOWN:
        {
            int32_t pulse = floorEncoderGetPulseCount();
            int32_t raw_delta = pulse - s_step_start_pulse;
            int32_t delta = (raw_delta < 0) ? -raw_delta : raw_delta;
            static uint32_t s_debug_tick = 0;

            if (delta >= (int32_t)FLOOR_15CM_PULSE) {
                motorStop();
                s_current_floor--;

                if (s_current_floor == s_target_floor) {
                    s_state = STATE_ARRIVED;
                } else {
                    elevatorControllerStartStep(s_target_floor);
                }
            } else if ((HAL_GetTick() - s_step_start_tick) > MOTOR_MOVE_TIMEOUT_MS) {
                motorStop();
                s_state = STATE_ERROR;
            } else if ((HAL_GetTick() - s_debug_tick) > 300U) {
                /* TODO(임시 디버그): 펄스가 실제로 움직이는지 확인용. 원인 파악되면 지울 것 */
                s_debug_tick = HAL_GetTick();
                printf("MOVING_DOWN debug: pulse=%ld delta=%ld / target=%u\r\n", (long)pulse, (long)delta, (unsigned)FLOOR_15CM_PULSE);
            }
        }
        break;

    case STATE_ARRIVED:
        buzzerPlayFloorTone(s_target_floor);
        printf("arrived at floor %u\r\n", s_current_floor);

        schedulerClearRequest(s_target_floor);
        s_state = STATE_IDLE;
        break;

    case STATE_ERROR:
        printf("MOVE TIMEOUT! stuck near floor %u (target %u)\r\n", s_current_floor, s_target_floor);
        schedulerClearRequest(s_target_floor); 
        s_state = STATE_IDLE;
        break;

    default:
        s_state = STATE_IDLE;
        break;
    }
}
