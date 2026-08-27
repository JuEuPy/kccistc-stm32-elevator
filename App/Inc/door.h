#ifndef APP_INC_DOOR_H_
#define APP_INC_DOOR_H_

/*
 * 외문(슬라이딩) 제어. 서보모터 1개로 구동.
 * 서보 자체는 각도 출력(0~90도)이며, 그 회전을 슬라이딩 도어 기구가 직선 개폐로 변환한다.
 */

// 문(서보모터)의 상태를 관리하는 ENUM
typedef enum {
    DOOR_STATE_CLOSED,
    DOOR_STATE_OPEN
} DoorState_t;

void doorInit(void);
void doorOpen(void);
void doorClose(void);

DoorState_t doorGetState(void);

#endif /* APP_INC_DOOR_H_ */
