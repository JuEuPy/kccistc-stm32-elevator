#ifndef APP_INC_APPMAIN_H_
#define APP_INC_APPMAIN_H_

/* main.c의 초기화부에서 1회 호출 */
void appInit(void);

/* main.c의 while(1) 루프 안에서 반복 호출 */
void appRun(void);

/* 모터 배선/방향 확인용 수동 테스트: 상승 -> 정지 -> 하강 -> 정지 순서로 실행 */
void appTestMotor(void);

/* 서보 도어 배선/각도 확인용 수동 테스트: 0->90->180->90->0도 순서로 1회 실행 */
void appTestDoor(void);

/* 초음파 센서 배선 확인용 수동 테스트: 거리(cm)를 계속 측정해 printf로 출력 (무한 루프) */
void appTestFloorSensor(void);

// 펄스 측정 테스트
void appTestFloorEncoder(void);
void appTestFloorEncoderCalibration(void);

void appTestDisplay(void);

#endif /* APP_INC_APPMAIN_H_ */
