/*
 * 使用するボードはArduino Mega 2560です
 * K, A, C, LPORTに8bit R-2R DACを接続し、コンパレータに入力して正弦波・三角波比較をハードウェア的に実現します
 */

#include <TimerOne.h>

#define DDR_ALLOUTPUT 0b11111111
#define BaudRate        115200

float Tau = 2 * PI;

uint8_t sinWave[1000];

void setup() {
  Serial.begin(BaudRate);

  DDRK = DDR_ALLOUTPUT;  //三角波出力ポート:
  DDRA = DDR_ALLOUTPUT;  //U相正弦波出力ポート:
  DDRC = DDR_ALLOUTPUT;  //V相正弦波出力ポート:
  DDRL = DDR_ALLOUTPUT;  //W相正弦波出力ポート:

  for(uint16_t i = 0; i < sizeof(sinWave); i++){
    sinWave[i] = (sin(Tau / sizeof(sinWave)) * 127.75) + 128;
    Serial.println("sinWave[" + String(i) + "] = " + string(sinWave[i]));
  }
}

void loop() {
  
}
