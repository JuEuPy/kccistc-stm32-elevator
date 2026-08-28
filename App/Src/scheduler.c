#include "scheduler.h"
#include "config.h"

/*
 * 버튼 누른 순서를 그대로 보존하는 FIFO 큐 (최대 SCHEDULER_QUEUE_CAPACITY개).
 */
static uint8_t s_queue[SCHEDULER_QUEUE_CAPACITY];
static uint8_t s_queue_count = 0;

void schedulerInit(void){
    s_queue_count = 0;
}

bool schedulerAddRequest(uint8_t floor){
    uint8_t i;

    if (floor < FLOOR_MIN || floor > FLOOR_MAX) {
        return false;
    }

    // 이미 큐에 있는 층이면 중복 등록하지 않는다 
    for (i = 0; i < s_queue_count; i++) {
        if (s_queue[i] == floor) {
            return false;
        }
    }

    if (s_queue_count >= SCHEDULER_QUEUE_CAPACITY) {
        return false;
    }

    s_queue[s_queue_count] = floor;
    s_queue_count++;
    return true;
}

bool schedulerHasPendingRequests(void){
    return s_queue_count > 0;
}

uint8_t schedulerGetNextTarget(uint8_t current_floor){
    if (s_queue_count == 0) {
        return current_floor;
    }

    return s_queue[0];
}

void schedulerClearRequest(uint8_t floor){
    uint8_t i, j;

    for (i = 0; i < s_queue_count; i++) {
        if (s_queue[i] == floor) {
            for (j = i; j < (uint8_t)(s_queue_count - 1); j++) {
                s_queue[j] = s_queue[j + 1];
            }
            s_queue_count--;
            return;
        }
    }
}
