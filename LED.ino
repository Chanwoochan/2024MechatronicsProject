/***** LED 사용 관련 함수들이 정의되어있는 스케치파일. ******/
/*****    국민대학교 기계공학부 20191089 김찬우.    ******/
/*****          kcwjoma@kookmin.ac.kr          ******/

#include "defpins.h"
#include "kcwgtj.h"

void ledOFF()             // LED 끄기
{
  digitalWrite(17,LOW);
  digitalWrite(18,LOW);
  digitalWrite(19,LOW);
}
void ledRGB(int R, int G, int B)             // LED 사용자 정의
{
  analogWrite(17,R);
  analogWrite(18,G);
  analogWrite(19,B);
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
