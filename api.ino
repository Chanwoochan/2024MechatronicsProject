/***** API 사용 관련 함수들이 정의되어있는 스케치파일. ******/
/*****    국민대학교 기계공학부 20191089 김찬우.    ******/
/*****          kcwjoma@kookmin.ac.kr          ******/

#include <WiFi.h>
#include <HTTPClient.h>
#include "defpins.h"
#include "kcwgtj.h"

WiFiClientSecure client;
HTTPClient https;
HTTPClient https2;

// 국민대의 날씨 정보를 API에서 가져오기.
const char* url = "https://api.openweathermap.org/data/2.5/weather?lat=37.61137821915271&lon=126.99419953626045&lang=kr&units=metric&appid=[[API 토큰은 비밀입니다.]]";
const char* timeUrl = "https://worldtimeapi.org/api/timezone/Asia/Seoul";
void httpInit() // API 서버에 연결
{
  client.setInsecure();
  client.connect(url, 443);
  https.begin(client, url);
  https2.begin(client, timeUrl);
}

void digit(double num)
{
  long TMP{(long)(abs(num * 100))};
  int a[4] {};
  a[0] = (TMP % 10000) / 1000;
  a[1] = (TMP % 1000) / 100;
  a[2] = (TMP % 100) / 10;
  a[3] = (TMP % 10);                 // 온도정보를 한글자 씩 분리. 
  if (num < 0) soundplay(36);       // 음수인경우, 사운드 재생 -> 영하
  playTrack(88);
  delay(600);
  if (a[1] != 0)                     
  {
    if (a[0] == 0) {}
    else if (a[0] == 1)
    {
      playTrack(12); //십
      delay(600);
    }
    else
    {
      playTrack(a[0] + 2);
      delay(600);
      playTrack(12); //십
      delay(600);
    }
    playTrack(a[1] + 2);
    delay(600);
  }
  else
  {
    if (a[0] == 0)
    {
      playTrack(2);
      delay(600);
    }
    else if (a[0] == 1)
    {
      playTrack(12); //십
      delay(600);
    }
    else
    {
      playTrack(a[0] + 2);
      delay(600);
      playTrack(12); //십
      delay(600);
    }
  }
  playTrack(13); //점
  delay(600);
  playTrack(a[2] + 2);
  delay(600);
  playTrack(a[3] + 2);
  delay(600);
}

void min_digit(int Min)
{
  int a[2] {};
  a[1] = Min    % 10;
  a[0] = Min/10 % 10;
  playTrack(88);
  delay(600);
  if (a[1] != 0)
  {
    if (a[0] == 0) {}
    else if (a[0] == 1)
    {
      playTrack(12); //십
      delay(600);
    }
    else
    {
      playTrack(a[0] + 2);
      delay(600);
      playTrack(12); //십
      delay(600);
    }
    playTrack(a[1] + 2);
    delay(600);
  }
  else
  {
    if (a[0] == 0)
    {
      playTrack(2);
      delay(600);
    }
    else if (a[0] == 1)
    {
      playTrack(12); //십
      delay(600);
    }
    else
    {
      playTrack(a[0] + 2);
      delay(600);
      playTrack(12); //십
      delay(600);
    }
  }
}


void wather()  // API 서버에서 날씨 정보 가져오기.
{
  client.connect(url, 443);
  if (WiFi.status() == WL_CONNECTED)
  {
    int httpCode = https.GET();                                         // API 서버에서 날씨 정보를 JSON 형식으로 받아옴.
    if (httpCode > 0)                                                   // 받아온 정보가 있으면,
    {
      String payload = https.getString();                               // 받아온 정보를 String 형식으로 반환
      int index1 = payload.indexOf("\"weather\":[{\"id\":");
      int index2 = payload.indexOf(",", index1);
      int wather = payload.substring(index1 + 17, index2).toInt();        // String data에서 필요한 정보 파싱. (날씨)
      index1 = payload.indexOf("\"temp\":");
      index2 = payload.indexOf(",", index1);
      double temp = payload.substring(index1 + 7, index2).toDouble();   // String data에서 필요한 정보 파싱. (기온)
      index1 = payload.indexOf("\"feels_like\":");
      index2 = payload.indexOf(",", index1);
      double feeltemp = payload.substring(index1 + 13, index2).toDouble();   // String data에서 필요한 정보 파싱. (채감온도)
      
      soundplay(17);                     // 사운드 재생 -> 현재 날씨는
      int wt;
      if     (wather == 800 || wather == 801) wt = 28; // 맑음
      else if               (wather/100 == 2) wt = 30;  // 뇌우
      else if               (wather/100 == 3) wt = 31;  // 이슬비
      else if               (wather/100 == 5) wt = 32;  // 비
      else if               (wather/100 == 6) wt = 33;  // 눈
      else if (wather > 801 && wather <= 805) wt = 29;  // 흐림
      else                                    wt = 34;  // 특이한 날씨
      soundplay(wt);                     // 사운드 재생 -> (날씨)
      soundplay(36);                     // 사운드 재생 -> 입니다.
      soundplay(18);                     // 사운드 재생 -> 현재 기온은
      digit(temp);
      soundplay(14);            // 사운드 재생 -> 도
      soundplay(36);            // 사운드 재생 -> 입니다.
      soundplay(19);                     // 사운드 재생 -> 채감온도는
      digit(feeltemp);
      soundplay(14);            // 사운드 재생 -> 도
      soundplay(36);            // 사운드 재생 -> 입니다.

    }
    https.end();
  }
}

void times()
{
  client.connect(timeUrl, 443);
  if (WiFi.status() == WL_CONNECTED)
  {
    int httpCode = https2.GET();                                         // API 서버에서 날씨 정보를 JSON 형식으로 받아옴.
    if (httpCode > 0)                                                   // 받아온 정보가 있으면,
    {
      String payload = https.getString();                               // 받아온 정보를 String 형식으로 반환
      int index1 = payload.indexOf("\"datetime\":");
      int index2 = payload.indexOf("T", index1);
      int H = payload.substring(index2+1, index2+3).toInt();
      int M = payload.substring(index2+4, index2+6).toInt();
      soundplay(1);
      soundplay2(18);
      if(H<12)
        soundplay2(3);
      else
        soundplay2(4); 
      soundplay2((int)(H%12 + 5));
      min_digit(M); 
      soundplay2(2);
      soundplay(36);
    }
    https2.end();
  }
}
