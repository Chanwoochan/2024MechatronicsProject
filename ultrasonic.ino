#include <NewPing.h>
#include "defpins.h"
#include "kcwgtj.h"

NewPing sonar(TRIG_PIN, ECHO_PIN, 300);        // 최대 측정 거리 : 300 cm

double ultrasonicDistence()                    // 측정 거리를 cm 단위로 출력
{
  return (double)(sonar.ping_cm());
}

double LPF{};
double usingLPF()                               // 측정한 거리에 LPF를 적용하여 cm 단위로 출력
{
  double distance = ultrasonicDistence();
  LPF = distance * 0.2 + LPF * 0.8;
  return LPF;
}

void outLedNBuzzer(double distance)             // 측정 거리를 LED로 표시 및 가까울 시 부저 작동
{
  if (distance > 80)
  {
    ledB();
    digitalWrite(PASSIVE_BUZZER_PIN, LOW);
  }
  else if (distance < 30)
  {
    ledR();
    digitalWrite(PASSIVE_BUZZER_PIN, HIGH);
  }
  else
  {
    ledG();
    digitalWrite(PASSIVE_BUZZER_PIN, LOW);
  }
}
