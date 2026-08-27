#ifndef APP_INC_FLOORSENSOR_H_
#define APP_INC_FLOORSENSOR_H_

#include <stdint.h>
#include <stdbool.h>

void floorSensorInit(void);

/*
 * 반환값이 false -> 타임아웃 또는 유효 범위 초과로 측정 실패
 */
bool floorSensorTrigger(void);

/* 가장 최근에 측정된 거리값(cm) */
uint16_t floorSensorGetDistanceCm(void);

/* 현재 층 */
uint8_t floorSensorGetCurrentFloor(void);

#endif /* APP_INC_FLOORSENSOR_H_ */
