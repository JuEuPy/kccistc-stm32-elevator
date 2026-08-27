#ifndef APP_INC_FLOORENCODER_H_
#define APP_INC_FLOORENCODER_H_

#include <stdint.h>

/* CubeMX에서 ENCODER_TIM을 Encoder Mode(Combined Channels)로 설정해둔 걸 가정 */
void floorEncoderInit(void);

/* 모터 축 엔코더의 누적 펄스 수 (TIM 카운터 값 그대로, 방향에 따라 증가/감소) */
int32_t floorEncoderGetPulseCount(void);

/* 누적 펄스 수를 config.h의 FLOOR_15CM_PULSE 기준으로 변환한 현재 층 */
uint8_t floorEncoderGetCurrentFloor(void);

#endif /* APP_INC_FLOORENCODER_H_ */
