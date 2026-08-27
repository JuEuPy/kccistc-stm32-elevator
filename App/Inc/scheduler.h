#ifndef APP_INC_SCHEDULER_H_
#define APP_INC_SCHEDULER_H_

#include <stdint.h>
#include <stdbool.h>

/* 목표 층 큐 초기화 */
void schedulerInit(void);

/* 호출 버튼 등에서 확정된 목표 층 등록 */
void schedulerAddRequest(uint8_t floor);

/* 대기 중인 목표 층 요청이 있는지 여부 */
bool schedulerHasPendingRequests(void);

/*
 * 현재 층을 기준으로 다음에 이동할 목표 층을 반환.
 * 반환값이 current_floor와 같으면 더 이상 이동할 목표가 없다는 의미.
 */
uint8_t schedulerGetNextTarget(uint8_t current_floor);

/* 목표 층 도착 처리(큐에서 제거) */
void schedulerClearRequest(uint8_t floor);

#endif /* APP_INC_SCHEDULER_H_ */
