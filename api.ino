#include <WiFi.h>
#include <HTTPClient.h>
#include "defpins.h"
#include "kcwgtj.h"

WiFiClientSecure client;
HTTPClient https;

// 국민대의 날씨 정보를 API에서 가져오기.
const char* url = "https://api.openweathermap.org/data/2.5/weather?lat=37.61137821915271&lon=126.99419953626045&lang=kr&units=metric&appid=d0eb77015eb64bcc1b07c347cc735fad";

void httpInit() // API 서버에 연결
{
  client.setInsecure();
  client.connect(url, 443);
  https.begin(client, url);
}

void wather()  // API 서버에서 날씨 정보 가져오기.
{
  if (WiFi.status() == WL_CONNECTED)
  {
    int httpCode = https.GET();                                         // API 서버에서 날씨 정보를 JSON 형식으로 받아옴.
    if (httpCode > 0)                                                   // 받아온 정보가 있으면,
    {
      String payload = https.getString();                               // 받아온 정보를 String 형식으로 반환
      int index1 = payload.indexOf("\"description\":");
      int index2 = payload.indexOf(",", index1);
      String wather = payload.substring(index1 + 15, index2 - 1);       // String data에서 필요한 정보 파싱. (날씨)
      index1 = payload.indexOf("\"temp\":");
      index2 = payload.indexOf(",", index1);
      double temp = payload.substring(index1 + 7, index2).toDouble();   // String data에서 필요한 정보 파싱. (기온)
//      Serial.print("현재 날씨는 ");
//      Serial.print(wather);
//      Serial.println(" 입니다.");
//      Serial.print("현재 기온은 ");
//      Serial.print(temp);
//      Serial.println(" ºC 입니다.");
      soundplay(19);                     사운드 재생 -> 현재 날씨는
      int wt;
      if (wather == "맑음") wt = 13;
      else if (wather == "흐림") wt = 19;
      else wt = 14;
      soundplay(wt);                     사운드 재생 -> (날씨)
      soundplay(16);                     사운드 재생 -> 입니다.
      long TMP{(long)(abs(temp * 100))};
      int a[4] {};
      a[0] = (TMP % 10000) / 1000;
      a[1] = (TMP % 1000) / 100;
      a[2] = (TMP % 100) / 10;
      a[3] = (TMP % 10);                 // 온도정보를 한글자 씩 분리. 
      soundplay(18);                     사운드 재생 -> 현재 온도는
      if (temp < 0) soundplay(24);       음수인경우, 사운드 재생 -> 영하
      if (a[1] != 0)                     
      {
        if (a[0] == 0) {}
        else if (a[0] == 1)
        {
          soundplay(15); //십
        }
        else
        {
          soundplay(a[0] + 2);
          soundplay(15); //십
        }
        soundplay(a[1] + 2);
      }
      else
      {
        if (a[0] == 0)
        {
          soundplay(2);
        }
        else if (a[0] == 1)
        {
          soundplay(15); //십
        }
        else
        {
          soundplay(a[0] + 2);
          soundplay(15); //십
        }
      }
      soundplay(17); //점
      soundplay(a[2] + 2);
      soundplay(a[3] + 2);
      soundplay(21);            // 사운드 재생 -> O십O점OO도
      
      soundplay(16);            // 사운드 재생 -> 입니다.
    }
    https.end();
  }
}
