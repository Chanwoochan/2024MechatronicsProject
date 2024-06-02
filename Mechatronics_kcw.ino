/***** 시각장애인 보조 지팡이 Main 스캐치 파일입니다. ******/
/*****    국민대학교 기계공학부 20191089 김찬우.    ******/
/*****          kcwjoma@kookmin.ac.kr          ******/

#include "defpins.h"
#include "kcwgtj.h"

const char* ssid     = "Gwak";
const char* password = "rhkrxowls";
//const char* ssid     = "kcw";
//const char* password = "63921883";

hw_timer_t* timer = NULL; // 내부 타이머 선언
//portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

volatile bool interruptFlag;    // 인터럽트 업데이트 대상 플래그
volatile bool interruptFlag2;    // 인터럽트 업데이트 대상 플래그
void IRAM_ATTR APIInterrupt()   // API 버튼 FALLING 시 call
{
  interruptFlag=true;           // 버튼 입력 시 플래그를 세운다.
}
void IRAM_ATTR APIInterrupt2()   // API 버튼 FALLING 시 call
{
  interruptFlag2=true;           // 버튼 입력 시 플래그를 세운다.
}

bool flag{};
unsigned long CNT{};
int AA{}; long TT{}; 
void IRAM_ATTR tones()   // API 버튼 FALLING 시 call
{
  flag=!flag;
  if(flag && (CNT<(TT/2) || (TT<20 && TT>0)))
    digitalWrite(TONE_PIN,1);
  else
    digitalWrite(TONE_PIN,0);
  CNT++;
  if(CNT>=TT) CNT=0;
}

void setup()
{
  Serial.begin(115200);         // 디버딩용 시리얼 포트 Baudrate = 115200 bps
  soundBegin(9600);             // MP3 모듈 통신용 (소프트웨어)시리얼 포트 Baudrate = 9600 bps (고정)
  
  servoInit(SERVO_PIN);         // servo 모터 PWM 가동
  
  delay(10);

  // pinMode set
  pinMode(    WATHER_API_SW, INPUT_PULLUP);
  pinMode(      TIME_API_SW, INPUT_PULLUP);
  pinMode(    SERVO_MODE_SW, INPUT_PULLUP);
  pinMode(    SPK_STATE_PIN,        INPUT);
  pinMode(        LED_R_PIN,       OUTPUT);
  pinMode(        LED_G_PIN,       OUTPUT);
  pinMode(        LED_B_PIN,       OUTPUT);
  pinMode(         TRIG_PIN,       OUTPUT);
  pinMode(         ECHO_PIN,        INPUT);
  pinMode(SERVO_CONTROL_PIN,        INPUT);
  pinMode(         TONE_PIN,       OUTPUT);

  MPU6050Init();
  
  delay(2000);

  timer = timerBegin(0, 80, true);             // 내부타이머 초기화, 분주비 80
  timerAttachInterrupt(timer, &tones, true);   // 타이머 인터럽트 발생 시 실행 할 ISR 설정
  timerAlarmWrite(timer, 1000, true);          // 타이머 카운트 설정, 1000(카운트)*8(분주비)/80MHz(ESP32 기본 내부 클럭) = 1 ms 주기의 thread time.
  
  setVolume(30);                                // MP3 모듈의 출력 볼륨 설정
  delay(1000);
  soundplay(21);                               // 사운드 출력 -> 와이파이에 연결중입니다.
  
  if (!wifi_set(ssid, password))               // 와이파이에 연결 시도 및 조건분기
  {
    soundplay(24); 
    httpInit();
    playTrack(99);
    soundplay(25);
    soundplay(22);                             // 연결 성공 시 API 서버에 연결하고 사운드출력 -> 와이파이 연결에 성공했습니다. 시스템을 시작합니다.
  }
  else
  {
    soundplay(23);
    while (true);                              // 연결 실패 시 사운드출력 -> 와이파이 연결에 실패했습니다. 시스템을 초기화 해주세요.   재생 후 무한루프
  }
  
  attachInterrupt(WATHER_API_SW, APIInterrupt, FALLING);            // API 버튼 인터럽트 설정. API버튼 핀 FALLING 시 인터럽트.
  attachInterrupt(TIME_API_SW, APIInterrupt2, FALLING);            // API 버튼 인터럽트 설정. API버튼 핀 FALLING 시 인터럽트.
  timerAlarmEnable(timer);                                          // 타이머 작동 시작
  xTaskCreatePinnedToCore(loop2, "Task1", 10000, NULL, 0, NULL, 0); // 0번 코어에서 loop2 스레드 가동.  -> loop는 코어1 에서 가동. (멀티프로세스)
}

bool CrashFlag{};
unsigned long now{};
void loop() // Core 1 에서 가동. (사운드 출력 관련)
{
  now = millis();
  // wather() 함수는 불가피한 이유로 딜레이 함수가 많이 들어가므로, 다른 Task들과 분리하여 Core1 에서 담당.
  if (interruptFlag)      // API 인터럽트 발생 시 세워진 플래그를 감지
  {
    soundplay(16);        // 사운드 재생 -> 날씨정보를 불러오는 중입니다.
    wather();             // API 에서 날씨정보를 불러와 재생
    interruptFlag=false;  // 플래그 회수
  }
  if (interruptFlag2)      // API 인터럽트 발생 시 세워진 플래그를 감지
  {
    soundplay(1);
    soundplay2(17);        // 사운드 재생 -> 현재 시각을 불러오는 중입니다.
    times();               // API 에서 시간정보를 불러와 재생
    interruptFlag2=false;  // 플래그 회수
  }
  if (CrashFlag)          // 충돌감지시 세워진 플래그 감지
  {
    soundplay(27);        // 충돌감지 사운드 재생
    CrashFlag=false;      // 플래그 회수
  }
  while (millis() < now + 10);                              // thread time = 10 ms
}

void loop2( void *pvParameters) // Core 0 에서 가동. (센싱, 버튼인터럽트 및 시리얼통신)
{
  unsigned long now{};
  bool flag{};
  while (1) {
    now = millis();

    servo();                                                  // 서보모터 관련 계산 및 동작
    get6050();                                                // IMU 센서값 불러오기 및 관련 동작 처리
    
    outLedNBuzzer(usingLPF());                                // 초음파센서로 거리측정 후 LED, 부저 동작
    
    if (!digitalRead(SERVO_MODE_SW) && !flag)                 // 서보모터 모드 변경 버튼 입력 감지.
    {
      servoStateUpdate();                                     // 서보모터의 모드 변경
      flag = true;                                            // 플래그를 세움
      delay(100);                                             // 채터링 방지를 위한 딜레이
    }
    if (digitalRead(SERVO_MODE_SW) && flag) flag = false;     // 버튼 입력 종료 시 플래그 회수
    
    if (Serial.available() > 0) Serial.write(Serial.read());  // 외부 전송받은 시리얼 데이터를 받아와 처리.

    while (millis() < now + 50);                              // thread time = 50 ms
  }
}
