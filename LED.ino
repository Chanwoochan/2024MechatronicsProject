#include "defpins.h"
#include "kcwgtj.h"

void ledOFF()             // LED 끄기
{
  digitalWrite(17,LOW);
  digitalWrite(18,LOW);
  digitalWrite(19,LOW);
}
void ledR()             // 빨강 LED 켜기
{
  digitalWrite(17,HIGH);
  digitalWrite(18,LOW);
  digitalWrite(19,LOW);
}
void ledG()             // 초록 LED 켜기
{
  digitalWrite(17,LOW);
  digitalWrite(18,HIGH);
  digitalWrite(19,LOW);
}
void ledB()             // 파랑 LED 켜기
{
  digitalWrite(17,LOW);
  digitalWrite(18,LOW);
  digitalWrite(19,HIGH);
}
void ledW()             // 모든 LED 켜기 (흰색)
{
  digitalWrite(17,HIGH);
  digitalWrite(18,HIGH);
  digitalWrite(19,HIGH);
}
