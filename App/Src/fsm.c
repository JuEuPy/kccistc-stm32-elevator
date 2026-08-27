#include "fsm.h"
#include "scheduler.h"
#include "motor.h"
#include "floor_sensor.h"

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
