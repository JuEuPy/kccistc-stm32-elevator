//미사용

// #include "floorSensor.h"
// #include "config.h"

// static uint16_t s_distance_cm = 0;

// static void dwtInit(void){
//     CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
//     DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
// }

// static void delayUs(uint32_t us){
//     uint32_t start = DWT->CYCCNT;
//     uint32_t ticks = us * (SystemCoreClock / 1000000U);

//     while ((DWT->CYCCNT - start) < ticks) {
//     }
// }
// bool floorSensorTrigger(void){
//     uint32_t wait_start;
//     uint32_t timeout_ticks;
//     uint32_t echo_start_tick;
//     uint32_t elapsed_ticks;
//     uint32_t duration_us;
//     uint32_t distance;

//     /* 1. TRIG 핀에 10us HIGH 펄스 인가 */
//     HAL_GPIO_WritePin(SENSOR_TRIG_GPIO_Port, SENSOR_TRIG_Pin, GPIO_PIN_SET);
//     delayUs(10);
//     HAL_GPIO_WritePin(SENSOR_TRIG_GPIO_Port, SENSOR_TRIG_Pin, GPIO_PIN_RESET);

//     /* 2. Echo가 HIGH로 올라올 때까지 대기 (센서 미연결/무응답 대비 타임아웃) */
//     wait_start = DWT->CYCCNT;
//     timeout_ticks = SENSOR_ECHO_START_TIMEOUT_US * (SystemCoreClock / 1000000U);
//     while (HAL_GPIO_ReadPin(SENSOR_ECHO_GPIO_Port, SENSOR_ECHO_Pin) == GPIO_PIN_RESET) {
//         if ((DWT->CYCCNT - wait_start) > timeout_ticks) {
//             return false;
//         }
//     }
//     echo_start_tick = DWT->CYCCNT;

//     /* 3. Echo가 LOW로 내려올 때까지 대기 (측정 범위 초과 대비 타임아웃) */
//     timeout_ticks = SENSOR_ECHO_END_TIMEOUT_US * (SystemCoreClock / 1000000U);
//     while (HAL_GPIO_ReadPin(SENSOR_ECHO_GPIO_Port, SENSOR_ECHO_Pin) == GPIO_PIN_SET) {
//         if ((DWT->CYCCNT - echo_start_tick) > timeout_ticks) {
//             return false;
//         }
//     }
//     elapsed_ticks = DWT->CYCCNT - echo_start_tick;

//     duration_us = elapsed_ticks / (SystemCoreClock / 1000000U);
//     distance = duration_us / 58U;

//     if ((distance < SENSOR_DISTANCE_MIN_CM) || (distance > SENSOR_DISTANCE_MAX_CM)) {
//         return false;
//     }

//     s_distance_cm = (uint16_t)distance;
//     return true;
// }

// // 거리 확인(cm)
// uint16_t floorSensorGetDistanceCm(void){
//     return s_distance_cm;
// }

// // 층 반환
// uint8_t floorSensorGetCurrentFloor(void){
//     uint16_t distance = floorSensorGetDistanceCm();
//     uint8_t floor;

//     for (floor = FLOOR_MIN; floor < FLOOR_MAX; floor++) {
//         if (distance <= (uint16_t)(FLOOR_HEIGHT_CM * floor)) {
//             return floor;
//         }
//     }

//     return FLOOR_MAX; 
// }
