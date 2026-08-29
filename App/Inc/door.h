#ifndef APP_INC_DOOR_H_
#define APP_INC_DOOR_H_

void doorInit(void);

/* 서보 각도(0~180도) 설정. 범위 밖 값은 0/180으로 clamp됨 */
void doorSetServoAngle(float angle);

#endif /* APP_INC_DOOR_H_ */
