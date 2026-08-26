# kccistc-stm32-elevator
Make! A! Elevator!
aa
# 핀 배치 (Pinout)

하드웨어가 확정되는 대로 이 표를 갱신하고, `App/Inc/config.h`의 정의도 함께 수정합니다.

## 층 호출 버튼 (택트 스위치)

| 층 | GPIO 포트/핀 | 비고 |
|----|--------------|------|
| 1층 | TBD | |
| 2층 | TBD | |
| 3층 | TBD | |

## 층 위치 감지 센서 (리드스위치/자석 또는 포토인터럽터)

| 층 | GPIO 포트/핀 | EXTI 라인 | 비고 |
|----|--------------|-----------|------|
| 1층 | TBD | TBD | |
| 2층 | TBD | TBD | |
| 3층 | TBD | TBD | |

## 모터 드라이버 (12V SE-DM185)

| 신호 | GPIO 포트/핀 | 비고 |
|------|--------------|------|
| IN1 (정회전) | TBD | |
| IN2 (역회전) | TBD | |
| ENA/PWM | TBD | 속도 제어 필요 시 |

## 기타

| 용도 | GPIO 포트/핀 | 비고 |
|------|--------------|------|
| 상태 표시 LED | TBD | 디버깅용 |


# 시스템 아키텍처

STM32 기반 3층 엘리베이터 미니프로젝트.
층수는 3층으로 시작하되, `App/Inc/config.h`의 `NUM_FLOORS` 값만 바꾸면 층수를 확장할 수 있도록 설계합니다.

## 모듈 구성

```
kccistc-stm32-elevator/
├── Core/                   # STM32CubeMX 생성 코드 (main.c, HAL 초기화 등)
│   ├── Inc/
│   └── Src/
├── Drivers/                # STM32CubeMX 생성 HAL/CMSIS 드라이버
├── App/                    # 엘리베이터 애플리케이션 로직 (직접 작성)
│   ├── Inc/
│   │   ├── config.h        # 핀 매핑, 층수 등 공통 상수
│   │   ├── button.h        # 층 호출 버튼 입력/디바운싱
│   │   ├── scheduler.h     # 목표 층 큐, 방향 결정
│   │   ├── motor.h         # 모터 구동 제어
│   │   ├── floor_sensor.h  # 층 위치 감지 센서
│   │   ├── fsm.h           # 엘리베이터 상태 머신
│   │   └── app_main.h      # 애플리케이션 진입점
│   └── Src/
│       ├── button.c
│       ├── scheduler.c
│       ├── motor.c
│       ├── floor_sensor.c
│       ├── fsm.c
│       └── app_main.c
├── CONTRIBUTING.md
└── README.md
```

`Core/`, `Drivers/`는 STM32CubeMX가 생성하는 영역이라 사람마다 결과물이 달라질 수 있으므로, 실제 하드웨어 로직은 전부 `App/`에 격리합니다. `main.c`는 `appInit()` / `appRun()`만 호출합니다.

## 모듈 책임

| 모듈 | 파일 | 책임 |
|------|------|------|
| Button | `button.c/h` | 1~3층 호출 버튼(택트 스위치) 입력을 주기적으로 스캔하고 디바운싱 처리, 확정된 입력을 Scheduler에 등록 |
| Scheduler | `scheduler.c/h` | 등록된 목표 층 큐 관리, 현재 층과 비교해 다음 목표 층 및 이동 방향 결정 |
| Motor | `motor.c/h` | 모터 드라이버를 통해 12V 모터 정회전(상승)/역회전(하강)/정지 제어 |
| Floor Sensor | `floor_sensor.c/h` | 각 층 위치 감지 센서(리드스위치+자석 또는 포토인터럽터) 인터럽트를 받아 현재 층 갱신 |
| FSM | `fsm.c/h` | IDLE / MOVING_UP / MOVING_DOWN / ARRIVED 상태 전이를 관리하며 Scheduler/Motor/Floor Sensor를 조합해 순차 제어 |
| App Main | `app_main.c/h` | 각 모듈 초기화 및 메인 루프(폴링) 구성, `main.c`에서 호출 |
