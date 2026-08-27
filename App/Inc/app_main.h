#ifndef APP_INC_APP_MAIN_H_
#define APP_INC_APP_MAIN_H_

/* main.c의 초기화부에서 1회 호출 */
void appInit(void);

/* main.c의 while(1) 루프 안에서 반복 호출 */
void appRun(void);

#endif /* APP_INC_APP_MAIN_H_ */
