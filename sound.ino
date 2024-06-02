/***** MP3 모듈 사용 관련 함수들이 정의되어있는 스케치파일. ******/
/*****       국민대학교 기계공학부 20191089 김찬우.      ******/
/*****             kcwjoma@kookmin.ac.kr            ******/

#include <SoftwareSerial.h>
#include "defpins.h"
#include "kcwgtj.h"

SoftwareSerial softSerial(/*rx =*/32, /*tx =*/33);

# define Start_Byte     0x7E      // 시작 값
# define Version_Byte   0xFF      // 버전 값
# define Command_Length 0x06      // 길이 값
# define End_Byte       0xEF      // 종료 값
# define Acknowledge    0x00      // 피드백 미 수신

void soundBegin(int baudrate)     // MP3 모듈과의 시리얼 통신을 위한 소프트웨어 시리얼통신 초기화.
{
  softSerial.begin(baudrate);
}

void setVolume(byte level)                            // 출력 볼륨 설정
{
  sendPacket(0x06, 0x00, level);
}

void playTrack(int16_t track)                         // 원하는 트랙 재생
{
  sendPacket(0x03, highByte(track), lowByte(track));
}

void playfTrack(int8_t folder,int8_t track)
{
  sendPacket(0x0F, folder, track);
}

void sendPacket(byte Commend, byte Par1, byte Par2)   // MP3 모듈에 패킷 전송
{
  int16_t a = (Par1 << 8) | Par2;
  Serial.println(a);
  int16_t checksum = -(Version_Byte + Command_Length + Commend + Acknowledge + Par1 + Par2);
  byte Command_line[10] = { Start_Byte, Version_Byte, Command_Length, Commend, Acknowledge, Par1, Par2, highByte(checksum), lowByte(checksum), End_Byte};
  for (byte k=0; k<10; k++) softSerial.write(Command_line[k]);
}

void soundplay(int8_t n)                                 // 사운드가 종료될 때 까지 대기하며 사운드 재생. BUSY 핀 사용
{
  playfTrack(1,n);
  delay(850);
  while(1) if (digitalRead(SPK_STATE_PIN)) break;
}

void soundplay2(int8_t n)                                 // 사운드가 종료될 때 까지 대기하며 사운드 재생. BUSY 핀 사용
{
  playfTrack(2,n);
  delay(850);
  while(1) if (digitalRead(SPK_STATE_PIN)) break;
}
