#ifndef APP_INC_FLOOR_SENSOR_H_
#define APP_INC_FLOOR_SENSOR_H_

#include <stdint.h>

void floorSensorInit(void);
uint8_t floorSensorGetCurrentFloor(void);

/* HAL_GPIO_EXTI_Callback()에서 호출 */
void floorSensorIrqHandler(uint16_t gpio_pin);

#endif /* APP_INC_FLOOR_SENSOR_H_ */
