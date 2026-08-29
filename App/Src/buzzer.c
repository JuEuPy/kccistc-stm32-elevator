#include "buzzer.h"
#include "config.h"
#include "tim.h"

void buzzerPlayTone(uint16_t frequency_hz, uint16_t duration_ms){
    uint32_t arr;

    if (frequency_hz == 0U) {
        return;
    }

    arr = (BUZZER_TIM_CLOCK_HZ / frequency_hz) - 1U;
    __HAL_TIM_SET_AUTORELOAD(&BUZZER_PWM_TIM, arr);         //음높이 결정
    __HAL_TIM_SET_COMPARE(&BUZZER_PWM_TIM, BUZZER_PWM_CHANNEL, arr / 10U);
    HAL_TIM_GenerateEvent(&BUZZER_PWM_TIM, TIM_EVENTSOURCE_UPDATE);

    HAL_TIM_PWM_Start(&BUZZER_PWM_TIM, BUZZER_PWM_CHANNEL); //출력
    HAL_Delay(duration_ms);
    HAL_TIM_PWM_Stop(&BUZZER_PWM_TIM, BUZZER_PWM_CHANNEL);

    //잔여 전압 클리어
    __HAL_TIM_SET_COMPARE(&BUZZER_PWM_TIM, BUZZER_PWM_CHANNEL, 0U);
}

void buzzerPlayFloorTone(uint8_t floor){
    switch (floor) {
        case 1:
            buzzerPlayTone(BUZZER_NOTE_DO_HZ, BUZZER_NOTE_DURATION_MS);
            break;
        case 2:
            buzzerPlayTone(BUZZER_NOTE_MI_HZ, BUZZER_NOTE_DURATION_MS);
            break;
        case 3:
            buzzerPlayTone(BUZZER_NOTE_SOL_HZ, BUZZER_NOTE_DURATION_MS);
            break;
        default:
            break;
    }
}
