#include <SoftwareSerial.h>
#include "defpins.h"
#include "kcwgtj.h"

const char* ssid     = "kcw";
const char* password = "63921883";

hw_timer_t* timer = NULL; // 내부 타이머 선언
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

volatile bool interruptFlag;    // 인터럽트 업데이트 대상 플래그
void IRAM_ATTR APIInterrupt()   // API 버튼 FALLING 시 call
{
  interruptFlag=true;           // 버튼 입력 시 플래그를 세운다.
}

void setup()
{
  Serial.begin(115200);         // 디버딩용 시리얼 포트 Baudrate = 115200 bps
  soundBegin(9600);             // MP3 모듈 통신용 (소프트웨어)시리얼 포트 Baudrate = 9600 bps (고정)
  
  servoInit(SERVO_PIN);         // servo 모터 PWM 가동
  
  delay(10);

  // pinMode set
  pinMode(WATHER_API_SW, INPUT_PULLUP);
  pinMode(SERVO_MODE_SW, INPUT_PULLUP);
  pinMode(SPK_STATE_PIN, INPUT);
  pinMode(LED_R_PIN, OUTPUT);
  pinMode(LED_G_PIN, OUTPUT);
  pinMode(LED_B_PIN, OUTPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(SERVO_CONTROL_PIN, INPUT);
  pinMode(PASSIVE_BUZZER_PIN, OUTPUT);
  
  delay(2000);

  // 내부 타이머 및 타이머인터럽트 초기화
  timer = timerBegin(0, 80, true);             // 내부타이머 초기화, 분주비 80
  timerAttachInterrupt(timer, &servo, true);   // 타이머 인터럽트 발생 시 실행 할 ISR 설정
  timerAlarmWrite(timer, 5000, true);          // 타이머 카운트 설정, 5000(카운트)*80(분주비)/80MHz(ESP32 기본 내부 클럭) = 5 ms 주기의 thread time.
  
  setVolume(2);                                // MP3 모듈의 출력 볼륨 설정
  soundplay(22);                               // 사운드 출력 -> 와이파이에 연결중입니다.
  
  if (!wifi_set(ssid, password))               // 와이파이에 연결 시도 및 조건분기
  {
    httpInit();
    soundplay(23);                             // 연결 성공 시 API 서버에 연결하고 사운드출력 -> 와이파이 연결에 성공했습니다. 시스템을 시작합니다.
  }
  else
  {
    soundplay(SPK_STATE_PIN);
    while (true);                              // 연결 실패 시 사운드출력 -> 와이파이 연결에 실패했습니다. 시스템을 초기화 해주세요.   재생 후 무한루프
  }
  
  attachInterrupt(WATHER_API_SW, APIInterrupt, FALLING);           // API 버튼 인터럽트 설정. API버튼 핀 FALLING 시 인터럽트.
  timerAlarmEnable(timer);                                         // 타이머 작동 시작
  xTaskCreatePinnedToCore(loop2, "Task1", 2048, NULL, 0, NULL, 0); // 0번 코어에서 loop2 스레드 가동.  -> loop는 코어1 에서 가동. (멀티프로세스)
}

void loop() // Core 1 에서 가동.
{
  // wather() 함수는 불가피한 이유로 딜레이 함수가 많이 들어가므로, 다른 Task들과 분리하여 Core1 에서 담당.
  if (interruptFlag)      // API 인터럽트 발생 시 세워진 플래그를 감지
  {
    soundplay(12);        // 사운드 재생 -> 날씨정보를 불러오는 중입니다.
    wather();             // API 에서 날씨정보를 불러와 재생
    interruptFlag=false;  // 플래그 회수
  }
}

void loop2( void *pvParameters) // Core 0 에서 가동.
{
  unsigned long now{};
  bool flag{};
  while (1) {
    now = millis();
    
    outLedNBuzzer(usingLPF());                                // 초음파센서로 거리측정 후 LED, 부저 동작
    
    if (!digitalRead(SERVO_MODE_SW) && !flag)                 // 서보모터 모드 변경 버튼 입력 감지.
    {
      servoStateUpdate();                                     // 서보모터의 모드 변경
      flag = true;                                            // 플래그를 세움
      delay(100);                                             // 채터링 방지를 위한 딜레이
    }
    if (digitalRead(SERVO_MODE_SW) && flag) flag = false;     // 버튼 입력 종료 시 플래그 회수
    
    if (Serial.available() > 0) Serial.write(Serial.read());  // 고글의 시리얼 데이터를 받아와 처리.
    
    while (millis() < now + 50);                              // thread time = 50 ms
  }
}
