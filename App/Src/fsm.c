#include "fsm.h"
#include "scheduler.h"
#include "motor.h"
#include "floor_sensor.h"

static ElevatorState_t s_state;

void FSM_Init(void)
{
    /* TODO */
}

void FSM_Update(void)
{
    /* TODO */
}

ElevatorState_t FSM_GetState(void)
{
    return s_state;
}
