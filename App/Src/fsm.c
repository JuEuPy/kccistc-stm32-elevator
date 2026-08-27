#include "fsm.h"
#include "scheduler.h"
#include "motor.h"
#include "config.h"

/*
 * config.h의 FLOOR_SENSE_METHOD에 따라 층 감지 방식을 컴파일타임에 하나만 선택한다.
 * fsmGetCurrentFloor()를 통해서만 접근하고, 아래 매크로 밖에서는 두 모듈을 직접 부르지 않는다.
 */
#if FLOOR_SENSE_METHOD == FLOOR_SENSE_ENCODER
#include "floorEncoder.h"
#define fsmGetCurrentFloor() floorEncoderGetCurrentFloor()
#else
#include "floorSensor.h"
#define fsmGetCurrentFloor() floorSensorGetCurrentFloor()
#endif

static ElevatorState_t s_state;

void fsmInit(void)
{
    /* TODO */
}

void fsmUpdate(void)
{
    /* TODO */
}

ElevatorState_t fsmGetState(void)
{
    return s_state;
}
