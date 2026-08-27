#include "appMain.h"
#include "button.h"
#include "scheduler.h"
#include "motor.h"
#include "floorSensor.h"
#include "floorEncoder.h"
#include "fsm.h"
#include "main.h"
#include "usart.h"
#include <stdio.h>

int __io_putchar(int ch){
    HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
    return ch;
}

void appInit(void){
    /* TODO */
}

void appRun(void){
    /* TODO */
}

/*
 * 모터 배선/방향이 맞는지 확인하기 위한 수동 테스트.
 * FSM/스케줄러 없이 motor.c의 API만 직접 호출해서 상승 -> 정지 -> 하강 -> 정지를 실행
 */
void appTestMotor(void)
{
    motorInit();

    // motorUp();
    // HAL_Delay(1000);

    // motorStop();
    // HAL_Delay(500);

     motorDown();
     HAL_Delay(1000);

    motorStop();
}

/*
 * 초음파 센서 배선 확인용 수동 테스트.
 */
void appTestFloorSensor(void)
{
    floorSensorInit();

    while (1) {
        if (floorSensorTrigger()) {
            printf("거리: %u cm\r\n", floorSensorGetDistanceCm());
        } else {
            printf("거리: timeout/out of range\r\n");
        }

        HAL_Delay(300);
    }
}

/*
 * 모터 축 엔코더 배선 확인용 수동 테스트.
 */
void appTestFloorEncoder(void)
{
    floorEncoderInit();

    while (1) {
        printf("=====pulse: %ld\r\n", (long)floorEncoderGetPulseCount());
        HAL_Delay(300);
    }
}
