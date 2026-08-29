#include "display.h"
#include "bitmap.h" 
#include <stdio.h>   
#include "i2c.h"
#include "stm32f4xx_hal_i2c.h"
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

/**
 * @brief 디바이스 응답 확인 및 SSD1306 초기화 함수
 */

static void writeCommand(uint8_t cmd){
    HAL_I2C_Mem_Write(&hi2c3, SSD1306_I2C_ADDR ,  0x00,  1,  &cmd,  1,  10);
}

uint8_t ssd1306_buffer[SSD1306_WIDTH * SSD1306_HEIGHT / 8];

bool ssd1306Init(void)
{
    // 디바이스 응답 확인
    if (HAL_I2C_IsDeviceReady(&hi2c3, SSD1306_I2C_ADDR, 2, 10) != HAL_OK)
    {
        return false;
    }

    HAL_Delay(10);

    // SSD1306 초기화 시퀀스
    writeCommand(0xAE); // Display OFF

    writeCommand(0x20); // Set Memory Addressing Mode
    writeCommand(0x00); // 00: Horizontal Addressing Mode

    writeCommand(0xB0); // Set Page Start Address for Page Addressing Mode, 0-7

    writeCommand(0xC8); // Set COM Output Scan Direction (Reversed)
    writeCommand(0x00); // Set low column address
    writeCommand(0x10); // Set high column address

    writeCommand(0x40); // Set start line address

    writeCommand(0x81); // Set contrast control register
    writeCommand(0xFF);

    writeCommand(0xA1); // Set Segment Re-map (0 to 127)

    writeCommand(0xA6); // Set Normal display

    writeCommand(0xA8); // Set multiplex ratio (1 to 64)
    writeCommand(0x3F); // 1/64 duty

    writeCommand(0xA4); // Output follows RAM content

    writeCommand(0xD3); // Set display offset
    writeCommand(0x00); // Not offset

    writeCommand(0xD5); // Set display clock divide ratio/oscillator frequency
    writeCommand(0xF0); // Set divide ratio

    writeCommand(0xD9); // Set pre-charge period
    writeCommand(0x22);

    writeCommand(0xDA); // Set com pins hardware configuration
    writeCommand(0x12);

    writeCommand(0xDB); // Set vcomh
    writeCommand(0x20); // 0.77xVcc

    writeCommand(0x8D); // Set DC-DC enable
    writeCommand(0x14); // Enable charge pump

    writeCommand(0xAF); // Display ON

    ssd1306Clear();
    ssd1306Update();

    return true;
}

void ssd1306DrawPixel(int16_t x, int16_t y, uint8_t color) {
    if (x < 0 || x >= SSD1306_WIDTH || y < 0 || y >= SSD1306_HEIGHT) {
        return;
    }
    if (color == SSD1306_COLOR_WHITE) {
        ssd1306_buffer[x + (y / 8) * SSD1306_WIDTH] |= (1 << (y % 8));
    } else {
        ssd1306_buffer[x + (y / 8) * SSD1306_WIDTH] &= ~(1 << (y % 8));
    }
}

void ssd1306DrawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint8_t color) {
    int16_t dx = abs(x1 - x0);
    int16_t sx = x0 < x1 ? 1 : -1;
    int16_t dy = -abs(y1 - y0);
    int16_t sy = y0 < y1 ? 1 : -1;
    int16_t err = dx + dy;
    int16_t e2;

    while (1) {
        ssd1306DrawPixel(x0, y0, color);
        if (x0 == x1 && y0 == y1)
            break;
        e2 = 2 * err;
        if (e2 >= dy) {
            err += dy;
            x0 += sx;
        }
        if (e2 <= dx) {
            err += dx;
            y0 += sy;
        }
    }
}

void ssd1306Clear(void) {
    memset(ssd1306_buffer, 0x00, sizeof(ssd1306_buffer));
}

void ssd1306Update(void) {
    writeCommand(0x21);
    writeCommand(0);
    writeCommand(SSD1306_WIDTH - 1);

    writeCommand(0x22);
    writeCommand(0);
    writeCommand((SSD1306_HEIGHT / 8) - 1);

    HAL_I2C_Mem_Write(&hi2c3, SSD1306_I2C_ADDR, 0x40, 1, ssd1306_buffer, sizeof(ssd1306_buffer), 100);
}

void ssd1306DrawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint8_t color) {
    ssd1306DrawLine(x, y, x + w - 1, y, color);
    ssd1306DrawLine(x, y + h - 1, x + w - 1, y + h - 1, color);
    ssd1306DrawLine(x, y, x, y + h - 1, color);
    ssd1306DrawLine(x + w - 1, y, x + w - 1, y + h - 1, color);
}

void ssd1306DrawBitmap(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint8_t color) {
    int16_t byteWidth = (w + 7) / 8;
    
    for (int16_t j = 0; j < h; j++) {
        for (int16_t i = 0; i < w; i++) {
            if (*(bitmap + j * byteWidth + i / 8) & (128 >> (i % 8))) {
                ssd1306DrawPixel(x + i, y + j, color);
            }
        }
    }
}

/**
 * @brief 문자에 해당하는 16x16 비트맵 포인터를 반환하는 헬퍼 함수
 */
static const uint8_t* getBitmapForChar(char c) {
    switch (c) {
        case '0': return num0Bitmap;
        case '1': return num1Bitmap;
        case '2': return num2Bitmap;
        case '3': return num3Bitmap;
        case '4': return num4Bitmap;
        case '5': return num5Bitmap;
        case '6': return num6Bitmap;
        case '7': return num7Bitmap;
        case '8': return num8Bitmap;
        case '9': return num9Bitmap;
        case 'F': return charFBitmap;
        case 'B': return charBBitmap;
        default:  return NULL;
    }
}

/**
 * @brief 엘리베이터 층 및 상태 표시 화면을 렌더링하는 함수 (16x16 대형 비트맵 적용)
 */
void drawElevatorScreen(int floor, ElevatorState state) {
    ssd1306Clear();

    // 화면 테두리 이중선
    ssd1306DrawRect(0, 0, SSD1306_WIDTH, SSD1306_HEIGHT, SSD1306_COLOR_WHITE);
    ssd1306DrawRect(2, 2, SSD1306_WIDTH - 4, SSD1306_HEIGHT - 4, SSD1306_COLOR_WHITE);

    // 층수 문자열 생성 (음수면 B, 양수면 F 붙임)
    char floorStr[16];
    if (floor < 0) {
        sprintf(floorStr, "B%d", -floor);
    } else {
        sprintf(floorStr, "%dF", floor);
    }

    int charLen = 0;
    while (floorStr[charLen] != '\0') {
        charLen++;
    }

    int bitmapWidth = 24;  // 가로 24픽셀
    int bitmapHeight = 28; // 세로 28픽셀로 변경
    int gap = 2; // 글자 간격

    int textTotalWidth = (charLen * bitmapWidth) + ((charLen - 1) * gap);
    int arrowSpace = (state != ELEVATOR_IDLE) ? (bitmapWidth + 4) : 0;

    // 전체 콘텐츠 가로 폭 및 시작 좌표 계산 (화면 중앙 정렬)
    int totalWidth = arrowSpace + textTotalWidth;
    int16_t startX = (SSD1306_WIDTH - totalWidth) / 2;
    
    // 수직 중앙 정렬 (필요시 뒤에 + 2 또는 + 4를 붙여 미세 조정 가능)
    int16_t contentY = (SSD1306_HEIGHT - bitmapHeight) / 2;

    int16_t currentX = startX;

    // 1. 상태가 IDLE이 아니면 화살표 출력
    if (state != ELEVATOR_IDLE) {
        if (state == ELEVATOR_UP) {
            ssd1306DrawBitmap(currentX, contentY, arrowUpBitmap, bitmapWidth, bitmapHeight, SSD1306_COLOR_WHITE);
        } else if (state == ELEVATOR_DOWN) {
            ssd1306DrawBitmap(currentX, contentY, arrowDownBitmap, bitmapWidth, bitmapHeight, SSD1306_COLOR_WHITE);
        }
        currentX += bitmapWidth + 4;
    }

    // 2. 층수 문자열을 순서대로 비트맵으로 출력
    for (int i = 0; i < charLen; i++) {
        const uint8_t *bmp = getBitmapForChar(floorStr[i]);
        if (bmp != NULL) {
            ssd1306DrawBitmap(currentX, contentY, bmp, bitmapWidth, bitmapHeight, SSD1306_COLOR_WHITE);
        }
        currentX += bitmapWidth + gap;
    }

    ssd1306Update();
}