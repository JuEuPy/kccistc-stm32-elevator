# kccistc-stm32-elevator

STM32 기반 3층 엘리베이터 제어 시스템 프로젝트입니다.

---

## 기술 스택

### Firmware & Development
<p>
  <img src="https://img.shields.io/badge/C-A8B9CC?style=for-the-badge&logo=c&logoColor=white" alt="C" />
  <img src="https://img.shields.io/badge/STM32-03234B?style=for-the-badge&logo=stmicroelectronics&logoColor=white" alt="STM32" />
  <img src="https://img.shields.io/badge/Git-F05032?style=for-the-badge&logo=git&logoColor=white" alt="Git" />
</p>

### 하드웨어 & 제어 부품
* **MCU 보드:** STM32 Nucleo 보드
* **센서 및 입출력:** 엔코더, 푸시 버튼, 상태 표시 LED, 부저

### 소프트웨어 설계 및 주요 기술
* **언어 & 프레임워크:** C언어 (C99), STM32 HAL 드라이버
* **논블로킹 스케줄링:** `HAL_GetTick()` 기반 주기적 틱(Tick) 스캔 및 디바운싱
* **층간 이동 스케줄러:** 요청 순차 처리
* **엔코더 위치 제어:** 타이머 하드웨어 쿼드러쳐 디코딩(Encoder Mode, 4채배)
* **시스템 제어:** 유한 상태 머신(FSM) 기반 엘리베이터 상태 전이
 
---

# 핀 배치 (Pinout)

하드웨어가 확정되는 대로 이 표를 갱신하고, `App/Inc/config.h`의 정의도 함께 수정합니다.

## 층 호출 버튼

| 층 | GPIO 포트/핀 | 아두이노/헤더 핀 | 모드 | 비고 |
|:---|:---|:---|:---|:---|
| 1층 | PC0 | A5 | GPIO_Input (Pull-up) | 버튼 누름 시 Low (Active-Low) |
| 2층 | PC1 | A4 | GPIO_Input (Pull-up) | 버튼 누름 시 Low (Active-Low) |
| 3층 | PC2 | CN7 (Pin 35) | GPIO_Input (Pull-up) | 버튼 누름 시 Low (Active-Low) |

### 각 층 승강장 호출 버튼 

| 호출 구분 | GPIO 포트/핀 | 아두이노/헤더 핀 | 모드 | 비고 |
|:---|:---|:---|:---|:---|
| 1층 호출 (상향) | PA3 | D0 | GPIO_Input (Pull-up) | 버튼 누름 시 Low (Active-Low) |
| 2층 호출 (하향) | PA2 | D1 | GPIO_Input (Pull-up) | 버튼 누름 시 Low (Active-Low) |
| 3층 호출 (하향) | PA10 | D2 | GPIO_Input (Pull-up) | 버튼 누름 시 Low (Active-Low) |
## 모터 드라이버 (L298N & SE-DM185)

| 신호 | GPIO 포트/핀 | 아두이노/헤더 핀 | 모드 | 비고 |
|:---|:---|:---|:---|:---|
| IN3 (정회전) | PB0 | A3 | GPIO_Output | High/Low 방향 제어 |
| IN4 (역회전) | PB1 | CN10 (Pin 24) | GPIO_Output | High/Low 방향 제어 |
| ENB (속도 PWM) | PB8 | D15 | TIM4_CH3 (PWM) | 1kHz 속도 제어 |
| A상 | PC6 | CN10(Pin 4) | TIM3_CH1 (Encoder Mode) | 방향 판별 |
| B상 | PC7 | D9 | TIM3_CH2 (Encoder Mode) | 방향 판별 |


## 서보 모터 (SG90 / MG996R 도어 제어)

| 신호 | GPIO 포트/핀 | 아두이노/헤더 핀 | 모드 | 비고 |
|:---|:---|:---|:---|:---|
| PWM | PA11 | D3 | TIM1_CH4 (PWM) | 50Hz (0°: 닫힘 / 90°: 열림) |

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
├── Core/                         # STM32CubeMX 생성 코드 (main.c, HAL 초기화 등)
│   ├── Inc/
│   └── Src/
├── Drivers/                      # STM32CubeMX 생성 HAL/CMSIS 드라이버
├── cmake/                        # STM32CubeMX 생성 CMake 빌드 설정
├── App/                          # 엘리베이터 애플리케이션 로직
│   ├── Inc/
│   │   ├── config.h              # 핀 매핑, 층수 등 공통 상수
│   │   ├── appMain.h             # 애플리케이션 진입점 
│   │   ├── button.h              # 층 호출 버튼 입력/디바운싱
│   │   ├── scheduler.h           # 목표 층 큐, 다음 목표 층 결정
│   │   ├── motor.h               # 모터 구동 제어
│   │   ├── door.h                # 서보 도어 개폐 제어
│   │   ├── buzzer.h              # 층 도착 알림음
│   │   ├── floorSensor.h         # 초음파 센서 기반 층 위치 감지
│   │   ├── floorEncoder.h        # 모터축 엔코더 기반 층 위치 감지
│   │   └── elevatorController.h  # 엘리베이터 상태 머신
│   └── Src/
│       ├── appMain.c
│       ├── button.c
│       ├── scheduler.c
│       ├── motor.c
│       ├── door.c
│       ├── buzzer.c
│       ├── floorSensor.c
│       ├── floorEncoder.c
│       └── elevatorController.c
└── README.md
```

`Core/`, `Drivers/`는 STM32CubeMX가 생성하는 영역이라 사람마다 결과물이 달라질 수 있으므로, 실제 하드웨어 로직은 전부 `App/`에 격리합니다. `main.c`는 `appInit()` / `appRun()`만 호출합니다.

## 모듈 책임

| 모듈 | 파일 | 책임 |
|------|------|------|
| Button | `button.c/h` | 1~3층 호출 버튼 입력을 주기적으로 스캔하고 디바운싱 처리, 확정된 입력을 Scheduler에 등록 |
| Scheduler | `scheduler.c/h` | 등록된 목표 층 큐 관리, 현재 층과 비교해 다음 목표 층 및 이동 방향 결정 |
| Motor | `motor.c/h` | 모터 드라이버를 통해 12V 모터 정회전(상승)/역회전(하강)/정지 제어 |
| Door | `door.c/h` | 서보 모터(SG90/MG996R) 각도를 제어해 도어 개폐 |
| Buzzer | `buzzer.c/h` | 층 도착 시 층별 음계(1층=도, 2층=미, 3층=솔) 알림음 재생 |
| Floor Sensor | `floorSensor.c/h` | 초음파 센서(HC-SR04)로 거리를 측정해 현재 층 판단 |
| Floor Encoder | `floorEncoder.c/h` | 모터축 엔코더 누적 펄스 수로 현재 층 판단 |
| Elevator Controller | `elevatorController.c/h` | IDLE / MOVING_UP / MOVING_DOWN / ARRIVED / ERROR 상태 전이를 관리하며 Scheduler/Motor/Floor 센서를 조합해 순차 제어 |
| App Main | 각 모듈 초기화 및 메인 루프(폴링) 구성, `main.c`에서 호출 |
