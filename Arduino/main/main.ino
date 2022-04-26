/*
 * 使用するボードはArduino Mega 2560です
 * K, A, C, LPORTに8bit R-2R DACを接続し、コンパレータに入力して正弦波・三角波をハードウェア的に実現します
 */

#define DDR_ALLOUTPUT 0b11111111
#define BaudRate        115200

uint8_t sinWave[360];

void setup() {
  Serial.begin(BaudRate);

  DDRK = DDR_ALLOUTPUT;  //三角波出力ポート:
  DDRA = DDR_ALLOUTPUT;  //U相正弦波出力ポート:
  DDRC = DDR_ALLOUTPUT;  //V相正弦波出力ポート:
  DDRL = DDR_ALLOUTPUT;  //W相正弦波出力ポート:

  for(uint16_t i = 0; i < sizeof(sinWave); i++){
    float theta = i * (PI / 180);
    sinWave[i]  = floor((sin(theta) * 127.9) + 128);
    Serial.println("sinWave[" + String(i) + "] = " + String(sinWave[i]));
  }
}

void loop() {
  //とりあえずD-PORTから60Hz正弦波を出力させる:
  for(uint16_t i = 0; i < sizeof(sinWave); i++){
    PORTK = sinWave[i];
    delayMicroseconds(46);
  }
}
