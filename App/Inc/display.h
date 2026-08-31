#ifndef DISPLAY_H
#define DISPLAY_H

#include "stm32f4xx_hal.h"
#include <stdint.h>
#include <stdbool.h>
#include "elevatorController.h"

#define SSD1306_WIDTH  128
#define SSD1306_HEIGHT 64
#define SSD1306_I2C_ADDR (0x3C << 1)

#define SSD1306_COLOR_WHITE 1
#define SSD1306_COLOR_BLACK 0



// extern을 사용하여 선언만 하고, 실제 정의는 display.c에서 관리
extern uint8_t ssd1306_buffer[SSD1306_WIDTH * SSD1306_HEIGHT / 8];

bool ssd1306Init(void);
void ssd1306Clear(void);
void ssd1306Update(void);
void ssd1306DrawPixel(int16_t x, int16_t y, uint8_t color);
void ssd1306DrawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint8_t color);
void ssd1306DrawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint8_t color);
void ssd1306DrawBitmap(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint8_t color);

// 누락되었던 엘리베이터 화면 함수 선언 추가
void drawElevatorScreen(int floor, ElevatorState_t state);

#endif


