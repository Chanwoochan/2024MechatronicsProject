/***** 초음파센서 사용 관련 함수들이 정의되어있는 스케치파일. ******/
/*****       국민대학교 기계공학부 20191089 김찬우.       ******/
/*****             kcwjoma@kookmin.ac.kr             ******/

#include <NewPing.h>
#include "defpins.h"
#include "kcwgtj.h"

NewPing sonar(TRIG_PIN, ECHO_PIN, 300);        // 최대 측정 거리 : 300 cm

double ultrasonicDistence()                    // 측정 거리를 cm 단위로 출력
{
  double dist{};
  dist = sonar.ping_cm();
  if(dist<0.1) dist = 300;
  return dist;
}

double LPF{};
double usingLPF()                               // 측정한 거리에 LPF를 적용하여 cm 단위로 출력
{
  double distance = ultrasonicDistence();
  LPF = distance * 0.25 + LPF * 0.75;
  return LPF;
}

void outLedNBuzzer(double distance)             // 측정 거리를 LED로 표시 및 가까울 시 부저 작동
{
  if (distance > 80)
  {
    ledRGB(0, (int)((300 - distance)/220. * 255.), (int)((distance-80)/220. * 255.));
    TT=0;
  }
  else
  {
    ledRGB((int)((80 - distance)/80. * 255.), (int)(distance/80. * 255.),0);
    TT = (long)((((distance+10.)/90.)*100000.)/AA);
  }
}
