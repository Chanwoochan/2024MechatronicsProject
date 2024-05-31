#include <ESP32_Servo.h>
#include "defpins.h"
#include "kcwgtj.h"

Servo myservo;

void servoInit(int pin)     // 서보모터 초기화
{
   myservo.attach(pin);
}

int servoState{};
void servoStateUpdate()     // 서보모터 모드 업데이트
{
  servoState++;
  if (servoState > 2)servoState = 0;
}

int a{}; bool b{};
void IRAM_ATTR servo()    // 서보모터 제어 스레드 threadtime = 5 ms
{
  if (servoState == 0)                                  // 모드 0 : 정면 고정
    a = 90;
  else if (servoState == 1)                             // 모드 1 : 사용자 제어
    a = analogRead(SERVO_CONTROL_PIN) / 4095. * 180.;
  else                                                  // 모드 2 : 레이더 모드
  {
    if (a >= 180)
      b = true;
    else if (a <= 0)
      b = false;
    if (!b)
      a++;
    else
      a--;
  }
  myservo.write(a);                                      // 서보모터에 각도 반영
}
