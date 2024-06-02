/***** MPU6050 사용 관련 함수들이 정의되어있는 스케치파일. ******/
/*****      국민대학교 기계공학부 20191089 김찬우.       ******/
/*****            kcwjoma@kookmin.ac.kr             ******/
#include<Wire.h>
#include "defpins.h"
#include "kcwgtj.h"

short AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;

void MPU6050Init()
{
  Wire.begin();
  Wire.beginTransmission(0x68);
  Wire.write(0x6B);
  Wire.write(0);//MPU6050 을 동작 대기 모드로 변경
  Wire.endTransmission(true);
}

short OlD{};
void get6050()
{
  Wire.beginTransmission(0x68);//MPU6050 호출
  Wire.write(0x3B);//AcX 레지스터 위치 요청
  Wire.endTransmission(false);
  Wire.requestFrom(0x68, 14, true);
  AcX = Wire.read() << 8 | Wire.read();
  AcY = Wire.read() << 8 | Wire.read();
  AcZ = Wire.read() << 8 | Wire.read();
  Tmp = Wire.read() << 8 | Wire.read();
  GyZ = Wire.read() << 8 | Wire.read();
  GyY = Wire.read() << 8 | Wire.read();
  GyZ = Wire.read() << 8 | Wire.read();
  if(abs(GyZ-OlD)>7000)
  {
    CrashFlag=true;
  }
  OlD = GyZ;
}
