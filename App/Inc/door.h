#ifndef APP_INC_DOOR_H_
#define APP_INC_DOOR_H_

#include <stdbool.h>

typedef enum {
    DOOR_CLOSED,
    DOOR_OPENING,
    DOOR_OPEN,
    DOOR_CLOSING,
} DoorState_t;

void doorInit(void);

/* 서보 각도(0~180도) 설정. 범위 밖 값은 0/180으로 clamp됨 */
void doorSetServoAngle(float angle);

/* 문 열기 시작. 이미 열려있거나 여닫는 중이면 무시 */
void doorOpen(void);

/* appRun()에서 매 tick 호출. 열림/대기/닫힘을 시간 기반 */
void doorUpdate(void);

DoorState_t doorGetState(void);

#endif /* APP_INC_DOOR_H_ */
