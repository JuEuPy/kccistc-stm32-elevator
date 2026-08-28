#ifndef APP_INC_BUZZER_H_
#define APP_INC_BUZZER_H_

#include <stdint.h>

void buzzerInit(void);

/* 톤을 duration_ms 동안 재생하고 정지 */
void buzzerPlayTone(uint16_t frequency_hz, uint16_t duration_ms);

/* 층 도착 알림음: 1층=도, 2층=미, 3층=솔 */
void buzzerPlayFloorTone(uint8_t floor);

#endif /* APP_INC_BUZZER_H_ */
