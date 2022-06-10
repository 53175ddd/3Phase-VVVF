/*
 * 使用するボードはArduino Mega 2560です
 * K, A, C, LPORTに8bit R-2R DACを接続し、コンパレータに入力して正弦波・三角波比較をハードウェア的に実現します
 */

#include <TimerOne.h>

#define DDR_ALLOUTPUT 0b11111111
#define BaudRate        115200
#define waveVol         1000
#define triWaveFreq     20        //必ず4の倍数を指定:

#define analogPort      6

float Tau = 2 * PI;

uint8_t uSin[waveVol];
uint8_t vSin[waveVol];
uint8_t wSin[waveVol];
uint8_t cTri[waveVol];

uint16_t sentPhase = 0;

void sendTriWave() {
  PORTK = cTri[sentPhase];
  sentPhase++;

  if(sentPhase >= waveVol) {
    sentPhase = 0;
  }
}

void setup() {
  Serial.begin(BaudRate);

  DDRK = DDR_ALLOUTPUT;  //三角波出力ポート:
  DDRA = DDR_ALLOUTPUT;  //U相正弦波出力ポート:
  DDRC = DDR_ALLOUTPUT;  //V相正弦波出力ポート:
  DDRL = DDR_ALLOUTPUT;  //W相正弦波出力ポート:

  Timer1.initialize(10);
  Timer1.attachInterrupt(sendTriWave);

  for(uint16_t i = 0; i < waveVol; i++) {
    float theta = (i * Tau) / waveVol;
    uSin[i] = (sin(theta) * 126) + 128;
    Serial.print(String(uSin[i]) + ",");
  }
  Serial.println();
  for(uint16_t i = 0; i < waveVol; i++) {
    float theta = ((i * Tau) / waveVol) + (Tau / 3);
    vSin[i] = (sin(theta) * 126) + 128;
    Serial.print(String(vSin[i]) + ",");
  }
  Serial.println();
  for(uint16_t i = 0; i < waveVol; i++) {
    float theta = ((i * Tau) / waveVol) + ((Tau * 2) / 3);
    wSin[i] = (sin(theta) * 126) + 128;
    Serial.print(String(wSin[i]) + ",");
  }
  Serial.println();
  for(uint16_t i = 0; i < (triWaveFreq / 2); i++) {
    for(uint8_t o = 0; o < (waveVol / triWaveFreq); o++) {
      uint16_t phase = o + triWaveFreq * i * 2;
      cTri[phase] = (255 / (waveVol / triWaveFreq)) * o;
    Serial.print(String(cTri[phase]) + ",");
    }
    for(uint8_t o = (waveVol / triWaveFreq); o > 0; o--) {
      uint16_t phase = (o + 1) + triWaveFreq * i * 2;
      cTri[phase] = (255 / (waveVol / triWaveFreq)) * o;
    Serial.print(String(cTri[phase]) + ",");
    }
  }
  uint8_t buff[triWaveFreq / 4];
  for(uint8_t i = 0; i < (triWaveFreq / 4); i++) {
    buff[i] = cTri[i];
  }
  for(uint16_t i = 0; i < (waveVol - (triWaveFreq / 4)); i++) {
    cTri[i] = cTri[i + (triWaveFreq / 4)];
  }
  for(uint16_t i = 0; i < (triWaveFreq / 4); i++) {
    cTri[i + (waveVol - (triWaveFreq / 4)) + i] = buff[i];
  }
}

void loop() {
  for(uint16_t i = 0; i < waveVol; i++) {
    phaseOut(i);

    delayMicroseconds(map(analogRead(analogPort), 0, 1023, 50, 1000));
  }
}

void phaseOut(uint16_t phase) {
  //phase : wave's phase 0 - 999 ?
    PORTA = uSin[phase];
    PORTC = vSin[phase];
    PORTL = wSin[phase];
}
