#include "button.h"
#include "config.h"
#include "scheduler.h"
#include "elevatorController.h"
#include "main.h"

#include <stdbool.h>
#include <stdio.h>

typedef struct {
    GPIO_TypeDef *port;
    uint16_t pin;
    uint8_t floor;
    uint32_t press_start_tick;
    bool timing;     /* 눌림이 감지되어 안정화 시간을 재는 중인지 */
    bool confirmed;  /* 이번 눌림으로 이미 요청을 등록했는지 (뗄 때까지 재등록 방지) */
} Button_t;

static Button_t s_buttons[BTN_COUNT] = {
    /* 택트 스위치 1~3층 */
    { BTN_1_GPIO_Port, BTN_1_Pin, 1U, 0U, false, false },
    { BTN_2_GPIO_Port, BTN_2_Pin, 2U, 0U, false, false },
    { BTN_3_GPIO_Port, BTN_3_Pin, 3U, 0U, false, false },
    /* 홀 센서 버튼 1~3층 (택트와 버튼 종류만 다를 뿐, 로직은 동일) */
    { HALL_BTN_1_GPIO_Port,  HALL_BTN_1_Pin,  1U, 0U, false, false },
    { HALL_BTN_2_GPIO_Port,  HALL_BTN_2_Pin,  2U, 0U, false, false },
    { HALL_BTN_3_GPIO_Port,  HALL_BTN_3_Pin,  3U, 0U, false, false },
};

void buttonInit(void)
{
    /* TODO */
}

/*
 * appRun()에서 반복 호출됨. 호출 주기가 일정하지 않으므로(고정 tick 아님)
 * 호출 횟수가 아니라 HAL_GetTick() 기준 실제 경과시간으로 디바운싱한다.
 */
void buttonScan(void){
    for (uint8_t i = 0; i < BTN_COUNT; i++) {
        Button_t *btn = &s_buttons[i];
        bool pressed = (HAL_GPIO_ReadPin(btn->port, btn->pin) == GPIO_PIN_RESET);

        if (!pressed) {
            btn->timing = false;
            btn->confirmed = false;
            continue;
        }

        if (btn->confirmed) {
            continue;
        }

        if (!btn->timing) {
            btn->timing = true;
            btn->press_start_tick = HAL_GetTick();
            printf("버튼%u 눌렀다~~~~ \r\n", btn->floor);
            continue;
        }

        if ((HAL_GetTick() - btn->press_start_tick) >= BUTTON_DEBOUNCE_MS) {
            btn->confirmed = true;
            bool queued = schedulerAddRequest(btn->floor);
            printf("버튼%u 등록됬다구~~~, schedulerAddRequest=%d\r\n", btn->floor, (int)queued);
            if (queued) {
                elevatorControllerLightFloorLed(btn->floor); /* 큐에 등록된 순간 바로 점등 */
            }
        }
    }
}
