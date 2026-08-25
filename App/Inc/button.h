#ifndef APP_INC_BUTTON_H_
#define APP_INC_BUTTON_H_

#include <stdint.h>

/* 층 호출 버튼 초기화 (GPIO 설정은 Core에서 완료된 상태를 가정) */
void Button_Init(void);

/*
 * 주기적으로(APP_TICK_MS 간격) 호출.
 * 내부에서 디바운싱을 수행하고, 눌림이 확정되면 Scheduler_AddRequest()를 호출한다.
 */
void Button_Scan(void);

#endif /* APP_INC_BUTTON_H_ */
