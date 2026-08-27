#ifndef APP_INC_APPMAIN_H_
#define APP_INC_APPMAIN_H_

/* main.c의 초기화부에서 1회 호출 */
void appInit(void);

/* main.c의 while(1) 루프 안에서 반복 호출 */
void appRun(void);

/* 모터 배선/방향 확인용 수동 테스트: 상승 -> 정지 -> 하강 -> 정지 순서로 실행 */
void appTestMotor(void);

#endif /* APP_INC_APPMAIN_H_ */
