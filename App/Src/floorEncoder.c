#include "floorEncoder.h"
#include "config.h"
#include "tim.h"

void floorEncoderInit(void)
{
    /*
     * Encoder Mode로 시작하면 하드웨어가 A/B상 펄스를 자동으로 센다
     */
    HAL_TIM_Encoder_Start(&ENCODER_TIM, TIM_CHANNEL_ALL);
}

int32_t floorEncoderGetPulseCount(void)
{
    return (int32_t)__HAL_TIM_GET_COUNTER(&ENCODER_TIM);
}

uint8_t floorEncoderGetCurrentFloor(void)
{
    int32_t pulse_count = floorEncoderGetPulseCount();
    uint8_t floor;

    if (pulse_count < 0) {
        pulse_count = 0;
    }

    for (floor = FLOOR_MIN; floor < FLOOR_MAX; floor++) {
        if ((uint32_t)pulse_count <= FLOOR_15CM_PULSE * floor) {
            return floor;
        }
    }

    return FLOOR_MAX; /* 나머지는 최상층 */
}
