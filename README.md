# kccistc-stm32-elevator
Make! A! Elevator!
aa
# 핀 배치 (Pinout)

하드웨어가 확정되는 대로 이 표를 갱신하고, `App/Inc/config.h`의 정의도 함께 수정합니다.

## 층 호출 버튼

| 층 | GPIO 포트/핀 | 모드 | 비고
|:---|:---|:---|:---|:---|
| 1층 | PC0 / A5 | GPIO_INPUT(PULL UP) | 버튼 누름 시 Low
| 2층 | PC1 / A4 | GPIO_INPUT(PULL UP) | 버튼 누름 시 Low
| 3층 | PC2 / CN7(Pin 35) | GPIO_INPUT(PULL UP) | 버튼 누름 시 Low

## 층 위치 감지 센서 (HC-SR04 초음파 센서)

## 모터 드라이버(L298N & SE-DM185)

| 신호 | GPIO 포트/핀 | 모드 | 비고

| IN1 (정회전) | PB0 / A3 | GPIO_OUTPUT | High/Low
| IN2 (역회전) | PB1 / CN10(Pin 24) | GPIO_OUTPUT | High/Low
| ENA/PWM | PB8 / D15 | TIM4_CH3 (PWM) | 1kHz

## 서보 모터 (SG90 / MG996R)

| 신호 | GPIO 포트/핀 | 모드 | 비고
|:---|:---|:---|:---|:---|
| Trig | PA0 | A0 | GPIO_Output | 10µs 트리거 펄스 발신 |
| Echo | PA1 | A1 | GPIO_Input | 펄스 폭 측정 (실시간 거리 계산으로 층 판별) |

## 모터 드라이버 (L298N & SE-DM185)

| 신호 | GPIO 포트/핀 | 아두이노/헤더 핀 | 모드 | 비고 |
|:---|:---|:---|:---|:---|
| IN1 (정회전) | PB0 | A3 | GPIO_Output | High/Low 방향 제어 |
| IN2 (역회전) | PB1 | CN10 (Pin 24) | GPIO_Output | High/Low 방향 제어 |
| ENA (속도 PWM) | PB8 | D15 | TIM4_CH3 (PWM) | 1kHz 속도 제어 |

## 기타

| 용도 | GPIO 포트/핀 | 비고 |
|:---|:---|:---|:---|:---|
| 1층 LED(빨강) | PB6 / D10 | GPIO_OUTPUT | 디버깅용
| 2층 LED(노랑) | PA7 / D11 | GPIO_OUTPUT | 디버깅용
| 3층 LED(초록) | PA6 / D12 | GPIO_OUTPUT | 디버깅용


| 용도 | GPIO 포트/핀 | 아두이노/헤더 핀 | 모드 | 비고 |
|:---|:---|:---|:---|:---|
| 1층 LED (빨강) | PB6 | D10 | GPIO_Output | 1층 위치/호출 표시 |
| 2층 LED (노랑) | PA7 | D11 | GPIO_Output | 2층 위치/호출 표시 |
| 3층 LED (초록) | PA6 | D12 | GPIO_Output | 3층 위치/호출 표시 |
| Debug UART TX | PA2 | D1 | USART2_TX | 115200 bps 디버깅 출력 |
| Debug UART RX | PA3 | D0 | USART2_RX | 115200 bps 디버깅 입력 |


# 시스템 아키텍처

STM32 기반 3층 엘리베이터 미니프로젝트.
층수는 3층으로 시작하되, `App/Inc/config.h`의 `NUM_FLOORS` 값만 바꾸면 층수를 확장할 수 있도록 설계합니다.

## 모듈 구성
