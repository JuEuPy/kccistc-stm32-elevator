#ifndef APP_INC_FLOOR_SENSOR_H_
#define APP_INC_FLOOR_SENSOR_H_

#include <stdint.h>

void FloorSensor_Init(void);
uint8_t FloorSensor_GetCurrentFloor(void);

/* HAL_GPIO_EXTI_Callback()에서 호출 */
void FloorSensor_IRQHandler(uint16_t gpio_pin);

#endif /* APP_INC_FLOOR_SENSOR_H_ */
