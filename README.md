# ATmega128 테트리스
마이크로프로세서 Atmega128에 16x2 LCD 패널, 버튼, 그리고 시리얼 통신을 사용해서 만든 2인용 테트리스

![Overview](/images/Overview.jpg)

## 기술 스택

### Hardware
![ATmega128](https://img.shields.io/badge/ATmega128-007ACC?style=flat-square&logo=atmel&logoColor=white)

### Software
![Atmel Studio](https://img.shields.io/badge/Atmel%20Studio-7.0-red?style=flat-square)
![C](https://img.shields.io/badge/C-A8B9CC?style=flat-square&logo=c&logoColor=white)

## 기능 설명
- 테트리스 화면
    - 16x2 LCD의 3x2 글자칸을 이어붙여 테트리스를 구현할 수 있는 칸을 만듦
    - 정해진 타이머 이벤트마다 화면을 갱신하며 매 프레임이 게임 상황에 맞게 보여짐
- 게임 조작버튼
    - 블록 좌우 이동, 블록 회전 등을 조작하는 장치
    - ATmega 내부에서 인터럽트로 버튼 조작을 감지하며 비동기적으로 처리함으로써 입력으로 게임이 멈추지않고 매끄럽게 돌아가면서 즉각적인 인터렉션이 생기도록 함
- 통신 기능
    - 이 게임은 멀티플레이가 가능
    - 두 기기를 시리얼 통신으로 선을 연결하면 서로 그 기기를 감지하고, 게임을 동시에 시작하여 정해진 시간동안 점수가 높거나 끝가지 살아남은 쪽이 이기도록 함
    - 이 기능을 통해서 상대와 경쟁하며 더 재밌는 게임을 플레이할 수 있음

## 소스코드 파일 설명
- main.c
    - 메인 소스코드
    - lcd_library, delay, tetris를 include하여 lcd 제어, 딜레이, 테트리스 게임을 다룸
    - 버튼 인터럽트, 타이머 인터럽트, 시리얼 통신을 담당하여 외부 통신도 담당함
    - 그리하여 모든 기능들을 여기서 다루며 실질적인 동작을 행함
- lcd_library.c
    - 16x2 LCD 제어 소스코드
    - LCD 초기화, 딜레이, 커서 이동 등의 기초 기능을 다룸
    - 여기서 더 나아가 문자, 문자열 작성, 커스텀 문자 작성 등의 고급 기능까지 다룸
    - 이 기능을 main.c에서 사용하며 tetris.c의 map과 동기화하여 곧 게임 화면 랜더링으로 사용함
- tetris.c
    - 게임 시스템을 담당하는 소스코드
    - 게임 맵, 조작, 틱 등 테트리스 전반 시스템 기능을 담당함
    - main에서 타이머 인터럽트가 발생할 때마다 game_tick()을 실행해 게임을 진행함
    - 버튼 인터럽트와 통신 인터럽트에서 오는 반응을 곧 이 소스코드의 기능과 이어 게임 조작 및 제어를 실행함
- delay.c
    - 보편적인 딜레이 기능을 담당
    - 밀리초, 마이크로초 단위로 지연 기능을 제공함
