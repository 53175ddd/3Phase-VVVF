/*
 * 使用するボードはArduino Mega 2560です
 * K, A, C, LPORTに8bit R-2R DACを接続し、コンパレータに入力して正弦波・三角波比較をハードウェア的に実現します
 */

#include <TimerOne.h>

#define DDR_ALLOUTPUT   255
#define BaudRate        115200
#define waveVol         1024
#define triWaveFreq     12         //必ず4の倍数を指定:

#define analogPort      6

float Tau = 2 * PI;

uint8_t uSin[waveVol];
uint8_t vSin[waveVol];
uint8_t wSin[waveVol];
uint8_t cTri[waveVol];

uint16_t sentPhase = 0;

void sendTriWave() {
  digitalWrite(13, HIGH);
  PORTK = cTri[sentPhase];
  PORTA = uSin[sentPhase];
  sentPhase++;

  if(sentPhase >= waveVol) {
    sentPhase = 0;
  }
  digitalWrite(13,  LOW);
}

void setup() {
  pinMode(13, OUTPUT);

  Serial.begin(BaudRate);

  DDRK = DDR_ALLOUTPUT;  //三角波出力ポート:
  DDRA = DDR_ALLOUTPUT;  //U相正弦波出力ポート:
  DDRC = DDR_ALLOUTPUT;  //V相正弦波出力ポート:
  DDRL = DDR_ALLOUTPUT;  //W相正弦波出力ポート:

  for(uint16_t i = 0; i < waveVol; i++) {
    float theta = (i * Tau) / waveVol;
    uSin[i] = (sin(theta) * 126) + 128;
  }
  for(uint16_t i = 0; i < waveVol; i++) {
    float theta = ((i * Tau) / waveVol) + (Tau / 3);
    vSin[i] = (sin(theta) * 126) + 128;
  }
  for(uint16_t i = 0; i < waveVol; i++) {
    float theta = ((i * Tau) / waveVol) + ((Tau * 2) / 3);
    wSin[i] = (sin(theta) * 126) + 128;
  }

  const uint16_t offset = waveVol / (triWaveFreq * 4);

  for(uint16_t i = 0; i < offset; i++) {
    cTri[i] = map(i, 0, offset, 128, 255);
  }

  for(uint16_t i = offset; i < (offset * 3); i++) {
    cTri[i] = map(i - offset, 0, (offset * 2), 255, 0);
  }

  for(uint16_t i = (offset * 3); i < (offset * 4); i++) {
    cTri[i] = map(i - (offset * 3), 0, offset, 0, 127);
  }

  for(uint16_t i = 0; i < (waveVol - (offset * 4)); i++) {
    cTri[i + (offset * 4)] = cTri[i];
  }

  for(uint16_t i = 0; i < waveVol; i++) {
    Serial.print(String(uSin[i]) + ",");
  }
  Serial.println();
  for(uint16_t i = 0; i < waveVol; i++) {
    Serial.print(String(vSin[i]) + ",");
  }
  Serial.println();
  for(uint16_t i = 0; i < waveVol; i++) {
    Serial.print(String(wSin[i]) + ",");
  }
  Serial.println();
  for(uint16_t i = 0; i < waveVol; i++) {
    Serial.print(String(cTri[i]) + ",");
  }
  Serial.println();

  Timer1.initialize(10);
  Timer1.attachInterrupt(sendTriWave);
}

void loop() {
  /*
  for(uint16_t i = 0; i < waveVol; i++) {
    phaseOut(i);

    delayMicroseconds(map(analogRead(analogPort), 0, 1023, 50, 1000));
  }
   */
}

void phaseOut(uint16_t phase) {
  //phase : wave's phase 0 - 999 ?
    PORTA = uSin[phase];
    PORTC = vSin[phase];
    PORTL = wSin[phase];
}
