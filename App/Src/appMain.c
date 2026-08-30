#include "appMain.h"
#include "config.h"
#include "button.h"
#include "scheduler.h"
#include "motor.h"
#include "floorSensor.h"
#include "floorEncoder.h"
#include "elevatorController.h"
#include "door.h"
#include "main.h"
#include "stm32f4xx_hal_gpio.h"
#include "usart.h"
#include <stdio.h>
#include "display.h"

// Teleplot 통신용
int __io_putchar(int ch){
    HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
    return ch;
}

void appInit(void){
    buttonInit();
    schedulerInit();
    // elevatorControllerInit();
    // ssd1306Init();

    
}

/*
 * 버튼(BTN_1/2/3)과 홀센서 버튼(BTN_HALL_1/2/3) 모두 같은 요청 큐에 등록된다.
 */
void appRun(void){
    buttonScan();
    elevatorControllerUpdate();
    // appTestDisplay();

  //  drawElevatorScreen(2, STATE_MOVING_DOWN);
}

/*
 * 서보 도어 배선/각도 확인용 수동 테스트. 0 -> 180 -> 0도 순으로 움직여본다.
 */
void appTestDoor(void)
{
    doorInit();

    doorSetServoAngle(0);
    HAL_Delay(1000);

    doorSetServoAngle(180);
    HAL_Delay(1000);

    doorSetServoAngle(0);
    HAL_Delay(1000);
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

// void appTestDisplay(void){
   
//     // 디스플레이 초기화 확인
//     if (!ssd1306Init()) {
//         // 초기화 실패 시 처리 (필요한 경우 에러 LED 점등 등)
//         return;
//     }

//     // --- 3층 정지 화면 호출 ---
//     int currentFloor = 3;
//     drawElevatorScreen(currentFloor, ELEVATOR_IDLE);
    
//     // 화면이 꺼지지 않도록 유지 (테스트용)
//     while (1) {
//         // 필요에 따라 층수나 상태를 변경하는 로직 추가 가능
//     }
// }