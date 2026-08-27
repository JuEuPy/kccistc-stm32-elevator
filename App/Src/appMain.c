#include "appMain.h"
#include "config.h"
#include "button.h"
#include "scheduler.h"
#include "motor.h"
#include "floorSensor.h"
#include "floorEncoder.h"
#include "elevatorController.h"
#include "main.h"
#include "stm32f4xx_hal_gpio.h"
#include "usart.h"
#include <stdio.h>

int __io_putchar(int ch){
    HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
    return ch;
}

void appInit(void){
    schedulerInit();
    elevatorControllerInit();
}

/*
 * BTN_1=1층, BTN_2=2층, BTN_3=3층 호출 버튼을 읽어 목표 층으로 이동. 
 */
void appRun(void){
    uint8_t target_floor = 0;

    if (HAL_GPIO_ReadPin(BTN_1_GPIO_Port, BTN_1_Pin) == GPIO_PIN_RESET) {
        target_floor = 1;
    } else if (HAL_GPIO_ReadPin(BTN_2_GPIO_Port, BTN_2_Pin) == GPIO_PIN_RESET) {
        target_floor = 2;
    } else if (HAL_GPIO_ReadPin(BTN_3_GPIO_Port, BTN_3_Pin) == GPIO_PIN_RESET) {
        target_floor = 3;
    }

    if (target_floor == 0) {
        return;
    }

    /* 여러 층 요청을 큐에 쌓아 순서대로 처리하는 기능은 아직 미구현. 나중을 위해 요청만 기록해둠. */
    schedulerAddRequest(target_floor);

    elevatorControllerMoveToFloor(target_floor);
    printf("arrived at floor %u\r\n", elevatorControllerGetFloor());

    /* 버튼 뗄 때까지 대기 (중복 트리거 방지) */
    while (HAL_GPIO_ReadPin(BTN_1_GPIO_Port, BTN_1_Pin) == GPIO_PIN_RESET || HAL_GPIO_ReadPin(BTN_2_GPIO_Port, BTN_2_Pin) == GPIO_PIN_RESET 
        || HAL_GPIO_ReadPin(BTN_3_GPIO_Port, BTN_3_Pin) == GPIO_PIN_RESET) {
    }
}

/*
 * 모터 배선/방향이 맞는지 확인하기 위한 수동 테스트.
 * elevatorController/스케줄러 없이 motor.c의 API만 직접 호출해서 상승 -> 정지 -> 하강 -> 정지를 실행
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
 