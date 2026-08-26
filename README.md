# kccistc-stm32-elevator

STM32 기반 3층 엘리베이터 제어 시스템 프로젝트입니다.

---

# 핀 배치 (Pinout)

하드웨어가 확정되는 대로 이 표를 갱신하고, `App/Inc/config.h`의 정의도 함께 수정합니다.

## 층 호출 버튼

| 층 | GPIO 포트/핀 | 모드 | 비고
|:---|:---|:---|:---|:---|
| 1층 | PC0 / A5 | GPIO_INPUT(PULL UP) | 버튼 누름 시 Low
| 2층 | PC1 / A4 | GPIO_INPUT(PULL UP) | 버튼 누름 시 Low
| 3층 | PC2 / CN7(Pin 35) | GPIO_INPUT(PULL UP) | 버튼 누름 시 Low

### 각 층 승강장 호출 버튼 

| 층 | GPIO 포트/핀 | EXTI 라인 | 비고 |
|:---|:---|:---|:---|:---|
| 1층 | PA0 / A0 | EXTI Line 0 | 외부 인터럽트 |
| 2층 | PA1 / A1 | EXTI Line 1 | 외부 인터럽트| 
| 3층 | PA4 / A2  | EXTI Line 4 | 외부 인터럽트 |

## 모터 드라이버(L298N & SE-DM185)

| 신호 | GPIO 포트/핀 | 모드 | 비고

| IN1 (정회전) | PB0 / A3 | GPIO_OUTPUT | High/Low
| IN2 (역회전) | PB1 / CN10(Pin 24) | GPIO_OUTPUT | High/Low
| ENA/PWM | PB8 / D15 | TIM4_CH3 (PWM) | 1kHz

## 서보 모터 (SG90 / MG996R)

| 신호 | GPIO 포트/핀 | 모드 | 비고
|:---|:---|:---|:---|:---|
| PWM | PB3 / D3 | TIM2_CH2 (PWM) | 50hz? 

## 타이머 파라미터(CubeMX 기준)

TIM4 (DC 모터 속도 제어, 1kHz)
Prescaler (PSC): 100 - 1
Counter Period (ARR): 1000 - 1

TIM2 (서보 모터 도어 제어, 50Hz)
Prescaler (PSC): 1000 - 1
Counter Period (ARR): 2000 - 1


## 서보 모터 (SG90 / MG996R 도어 제어)

| 용도 | GPIO 포트/핀 | 비고 |
|:---|:---|:---|:---|:---|
| 1층 LED(빨강) | PB6 / D10 | GPIO_OUTPUT | 디버깅용
| 2층 LED(노랑) | PA7 / D11 | GPIO_OUTPUT | 디버깅용
| 3층 LED(초록) | PA6 / D12 | GPIO_OUTPUT | 디버깅용


## 상태 표시 LED & 시스템

| 용도 | GPIO 포트/핀 | 아두이노/헤더 핀 | 모드 | 비고 |
|:---|:---|:---|:---|:---|
| 1층 LED (초록) | PB6 | D10 | GPIO_Output | 1층 위치/호출 표시 |
| 2층 LED (초록) | PA7 | D11 | GPIO_Output | 2층 위치/호출 표시 |
| 3층 LED (초록) | PA6 | D12 | GPIO_Output | 3층 위치/호출 표시 |
| 도착 알림 부저 | PB9 | D14 | TIM11_CH1 | 수동 부저(KY-006) 층별 도착음(도/미/솔) | 

## 내부 디스플레이
| 신호 | GPIO 포트/핀 | 아두이노/헤더 핀 | 모드 | 비고 |
|:---|:---|:---|:---|:---|
| I2C3 | PB4 | D5 |  I2C3_SDA | 현재 층 표시용 디스플레이 |
| I2C3 | PA8 | D7 | I2C3_SCL |  |


## 타이머 파라미터 (CubeMX 기준 / 클럭 100MHz)

| 타이머 | 제어 대상 | 목표 주파수 | Prescaler (PSC) | Counter Period (ARR) | 출력 핀 |
|:---|:---|:---|:---|:---|:---|
| **TIM4** | DC 모터 속도 | 1kHz | `100 - 1` (99) | `1000 - 1` (999) | PB8 (CH3) |
| **TIM1** | 서보 모터 각도 | 50Hz | `1000 - 1` (999) | `2000 - 1` (1999) | PA11 (CH4) |

---


# 시스템 아키텍처

STM32 기반 3층 엘리베이터 미니프로젝트.  
층수는 3층으로 시작하되, `App/Inc/config.h`의 `NUM_FLOORS` 값만 바꾸면 층수를 확장할 수 있도록 테이블 기반으로 설계합니다.

## 모듈 구성

```
kccistc-stm32-elevator/
├── Core/                   # STM32CubeMX 생성 코드 (main.c, HAL 초기화 등)
│   ├── Inc/
│   └── Src/
├── Drivers/                # STM32CubeMX 생성 HAL/CMSIS 드라이버
├── App/                    # 엘리베이터 애플리케이션 로직 
│   ├── Inc/
│   │   ├── config.h        # 핀 매핑, 층수 등 공통 상수
│   │   ├── button.h        # 층 호출 버튼 입력/디바운싱
│   │   ├── scheduler.h     # 목표 층 큐, 방향 결정
│   │   ├── motor.h         # 모터 구동 제어
│   │   ├── floor_sensor.h  # 층 위치 감지 센서
│   │   ├── elevatorController.h  # 엘리베이터 상태/이동 제어
│   │   └── app_main.h      # 애플리케이션 진입점
│   └── Src/
│       ├── button.c
│       ├── scheduler.c
│       ├── motor.c
│       ├── floor_sensor.c
│       ├── elevatorController.c
│       └── app_main.c
├── CONTRIBUTING.md
└── README.md
```

`Core/`, `Drivers/`는 STM32CubeMX가 생성하는 영역이라 사람마다 결과물이 달라질 수 있으므로, 실제 하드웨어 로직은 전부 `App/`에 격리합니다. `main.c`는 `appInit()` / `appRun()`만 호출합니다.

## 모듈 책임

| 모듈 | 파일 | 책임 |
|------|------|------|
| Button | `button.c/h` | 1~3층 호출 버튼 입력을 주기적으로 스캔하고 디바운싱 처리, 확정된 입력을 Scheduler에 등록 |
| Scheduler | `scheduler.c/h` | 등록된 목표 층 큐 관리, 현재 층과 비교해 다음 목표 층 및 이동 방향 결정 |
| Motor | `motor.c/h` | 모터 드라이버를 통해 12V 모터 정회전(상승)/역회전(하강)/정지 제어 |
| Floor Sensor | `floor_sensor.c/h` | 각 층 위치 감지 센서(리드스위치+자석 또는 포토인터럽터) 인터럽트를 받아 현재 층 갱신 |
| Elevator Controller | `elevatorController.c/h` | IDLE / MOVING_UP / MOVING_DOWN / ARRIVED 상태 전이를 관리하며 Scheduler/Motor/Floor Sensor를 조합해 순차 제어 |
| App Main | `app_main.c/h` | 각 모듈 초기화 및 메인 루프(폴링) 구성, `main.c`에서 호출 |
