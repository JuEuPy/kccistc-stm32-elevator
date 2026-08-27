#include "elevatorController.h"
#include "scheduler.h"
#include "motor.h"
#include "config.h"

/*
 * config.h의 FLOOR_SENSE_METHOD에 따라 층 감지 방식을 하나만 선택한다.
 * elevatorControllerGetCurrentFloor()를 통해서만 접근하고, 아래 매크로 밖에서는 두 모듈을 직접 부르지 않는다.
 */
#if FLOOR_SENSE_METHOD == FLOOR_SENSE_ENCODER
#include "floorEncoder.h"
#define elevatorControllerGetCurrentFloor() floorEncoderGetCurrentFloor()
#else
#include "floorSensor.h"
#define elevatorControllerGetCurrentFloor() floorSensorGetCurrentFloor()
#endif

static ElevatorState_t s_state;
static uint8_t s_current_floor = FLOOR_MIN; /* 현재 층 위치, 기본값 1층에서 시작 */

void elevatorControllerInit(void){
    /* TODO */
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

#if FLOOR_SENSE_METHOD == FLOOR_SENSE_ENCODER
void elevatorControllerMoveToFloor(uint8_t target_floor){
    while (s_current_floor < target_floor) {
        int32_t start_pulse = floorEncoderGetPulseCount();

        motorUp();
        /* TODO: 모터/엔코더 이상 시 대비한 타임아웃 없음 */
        while ((floorEncoderGetPulseCount() - start_pulse) < (int32_t)FLOOR_15CM_PULSE) {
        }
        motorStop();

        s_current_floor++;
    }
}
#else
void elevatorControllerMoveToFloor(uint8_t target_floor){
    /* TODO: 초음파 모드용 구현 (elevatorControllerGetCurrentFloor()로 절대 층 비교하는 방식) */
    (void)target_floor;
}
#endif
