#include "appMain.h"
#include "button.h"
#include "scheduler.h"
#include "motor.h"
#include "floorSensor.h"
#include "fsm.h"
#include "main.h"

void appInit(void)
{
    /* TODO */
}

void appRun(void)
{
    /* TODO */
}

/*
 * 모터 배선/방향이 맞는지 확인하기 위한 수동 테스트.
 * FSM/스케줄러 없이 motor.c의 API만 직접 호출해서 상승 -> 정지 -> 하강 -> 정지를 실행
 */
void appTestMotor(void)
{
    motorInit();

    motorUp();
    HAL_Delay(1000);

    motorStop();
    HAL_Delay(500);

    motorDown();
    HAL_Delay(1000);
    
    motorStop();
}
