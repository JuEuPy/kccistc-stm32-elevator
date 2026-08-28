#include "scheduler.h"
#include "config.h"

/*
 * 층별 대기 요청 여부 (index 0 = FLOOR_MIN).
 * 값을 채우는 건 buttonScan()/appRun() 쪽이고, 실제 큐 순서 선택(schedulerGetNextTarget)은 아직 미구현.
 */
static bool s_pending[NUM_FLOORS];

void schedulerInit(void){
    for (uint8_t i = 0; i < NUM_FLOORS; i++) {
        s_pending[i] = false;
    }
}

void schedulerAddRequest(uint8_t floor){
    if (floor < FLOOR_MIN || floor > FLOOR_MAX) {
        return;
    }

    s_pending[floor - FLOOR_MIN] = true;
}

bool schedulerHasPendingRequests(void){
    /* TODO */
    return false;
}

uint8_t schedulerGetNextTarget(uint8_t current_floor){
    /* TODO */
    return current_floor;
}

void schedulerClearRequest(uint8_t floor){
    /* TODO */
}
