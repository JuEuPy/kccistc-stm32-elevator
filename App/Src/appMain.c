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
    /* TODO */
}

void appRun(void){
    /* TODO */
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


// void appTestFloorEncoderCalibration(void)
// {
//     floorEncoderInit();
//     motorInit();

//     motorUp();

//     while (HAL_GPIO_ReadPin(BTN_1_GPIO_Port, BTN_1_Pin) != GPIO_PIN_RESET) {
//         printf("pulse: %ld\r\n", (long)floorEncoderGetPulseCount());
//     }

//     motorStop();
//     printf("STOP! final pulse: %ld\r\n", (long)floorEncoderGetPulseCount());

//     while (1) {
//         /* 최종 값을 계속 볼 수 있도록 정지 상태 유지 */
//     }
// }

/*
 * 버튼 -> 목표 층 이동 직결 테스트 (elevatorController 안 거침).
 * BTN_1=1층, BTN_2=2층, BTN_3=3층. 현재 층을 기억해뒀다가,
 * 목표 층까지 필요한 만큼 한 스텝(FLOOR_15CM_PULSE)씩 반복 이동한다.
 * (motorUp()은 펄스가 감소, motorDown()은 증가하는 걸 실측으로 확인했으므로 부호를 그에 맞춤)
 */
void appTestButtonMove(void){
    uint8_t current_floor = FLOOR_MIN; /* 기본값 1층에서 시작 */

    floorEncoderInit();

    motorInit();

    while (1) {
        uint8_t target_floor = 0;

        if (HAL_GPIO_ReadPin(BTN_1_GPIO_Port, BTN_1_Pin) == GPIO_PIN_RESET) {
            target_floor = 1;
        } else if (HAL_GPIO_ReadPin(BTN_2_GPIO_Port, BTN_2_Pin) == GPIO_PIN_RESET) {
            target_floor = 2;
        } else if (HAL_GPIO_ReadPin(BTN_3_GPIO_Port, BTN_3_Pin) == GPIO_PIN_RESET) {
            target_floor = 3;
        }

        if (target_floor == 0) {
            continue;
        }

        while (current_floor < target_floor) {
            int32_t start_pulse = floorEncoderGetPulseCount();

            motorUp();
            while ((start_pulse - floorEncoderGetPulseCount()) < (int32_t)FLOOR_15CM_PULSE) {
            }
            motorStop();

            current_floor++;
        }

        while (current_floor > target_floor) {
            int32_t start_pulse = floorEncoderGetPulseCount();

            motorDown();
            while ((floorEncoderGetPulseCount() - start_pulse) < (int32_t)FLOOR_15CM_PULSE) {
            }
            motorStop();

            current_floor--;
        }

        printf("arrived at floor %u\r\n", current_floor);

        /* 버튼 뗄 때까지 대기 (중복 트리거 방지) */
        while (HAL_GPIO_ReadPin(BTN_1_GPIO_Port, BTN_1_Pin) == GPIO_PIN_RESET ||
               HAL_GPIO_ReadPin(BTN_2_GPIO_Port, BTN_2_Pin) == GPIO_PIN_RESET ||
               HAL_GPIO_ReadPin(BTN_3_GPIO_Port, BTN_3_Pin) == GPIO_PIN_RESET) {
        }
    }
}