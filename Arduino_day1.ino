#include "DHT.h"        // DHT 사용 
#include <U8g2lib.h>    // OLED 사용
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

#define DHTPIN A1       // 온/습도 - A1 pin
#define DHTTYPE DHT22   // DHT22 버전을 사용하겠다. 

#define SOILHUMI A6     // 토양 습도 - A6 pin

DHT dht(DHTPIN, DHTTYPE); // 핀 = A1 , 모델 = DHT22

uint32_t DataCaptureDelay = 3000; //3초마다 실행 
uint32_t DataCapture_ST = 0;       //실행시간 시간

float Temp;     //온도 사용 위한 선언
float Humi;     //습도 사용 위한 선언
int soil = 0;   //토양 습도확인

void setup(void) {
  //시리얼 통신 
  Serial.begin(9600);

  //온도 습도 + OELD + 토지 사용
  dht.begin();
  u8g2.begin();
  pinMode(SOILHUMI, INPUT);

  //3초 살행
  DataCapture_ST = millis(); 
}

void loop(void) {
  if((millis() - DataCapture_ST) > DataCaptureDelay){ // 매 3초마다 업데이트
    //각 필요한 정보 READ
    Humi = dht.readHumidity();
    Temp = dht.readTemperature();
    soil = map(analogRead(SOILHUMI),0,1023,100,0);
    
    if (isnan(Humi) || isnan(Temp)){ //에러 처리
      Serial.println(F("Failed to read from DHT sensor!"));
      return;
   }
  
  OLEDdraw();
  DataCapture_ST =millis();
  }  
}




void OLEDdraw(){
  //매 출력 시작할 때 초기화
  u8g2.clearBuffer();
  
  //폰트 지정 및 동준우 출력
  u8g2.setFont(u8g2_font_ncenB08_te);
  u8g2.drawStr(1, 15, "DonjunWoo");

  //온도
  u8g2.drawStr(15, 36, "Temp");
  u8g2.setCursor(85,36);
  u8g2.print(Temp);
  u8g2.drawStr(114,36,"\xb0");
  u8g2.drawStr(119,36,"C");

  //습도
  u8g2.drawStr(15,47,"Humidity");
  u8g2.setCursor(85,47);
  u8g2.print(Humi);
  u8g2.drawStr(116,47,"%");

  //토양 습도 체크
  u8g2.drawStr(15,58,"Soil Humi");
  u8g2.setCursor(85,58);
  u8g2.print(soil);
  u8g2.drawStr(116,58,"%");

  //버퍼 전송
  u8g2.sendBuffer();
}
