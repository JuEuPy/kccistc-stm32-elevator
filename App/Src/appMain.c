#include "appMain.h"
#include "button.h"
#include "scheduler.h"
#include "motor.h"
#include "floorSensor.h"
#include "floorEncoder.h"
#include "fsm.h"
#include "main.h"
#include "stm32f4xx_hal_gpio.h"
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

    // motoDown();
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

/*
 * FLOOR_15CM_PULSE 실측 보정용 테스트.
 * 모터를 구동하면서 동시에 pulse 값을 실시간으로 계속 출력하고, BTN_1을 누르는 순간 모터를 멈추고 그 시점의 최종 pulse 값을 출력한다.
 * 사용법: 1층에 케이지를 두고 이 함수를 실행 -> 자로 15cm 지점을 보다가 그 순간 BTN_1을 눌러서 정지 -> 화면에 찍힌 최종 pulse 값을
 * config.h의 FLOOR_15CM_PULSE에 그대로 대입.
 */
void appTestFloorEncoderCalibration(void)
{
    floorEncoderInit();

    motorInit();

    motorUp();

     
     while (1) {
        printf("pulse: %ld\r\n", (long)floorEncoderGetPulseCount());

        if(HAL_GPIO_ReadPin(BTN_1_GPIO_Port, BTN_1_Pin) == GPIO_PIN_RESET){
            break;
         
     }
     HAL_Delay(30);
    }
  motorStop();
    printf("STOP! final pulse: %ld\r\n", (long)floorEncoderGetPulseCount());

    while (1) {
        /* 최종 값을 계속 볼 수 있도록 정지 상태 유지 */
    }
}
