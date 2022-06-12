//8bit幅の三相正弦波とU相と同期させた搬送波をCSV形式で出力します:

#define BaudRate        115200
#define waveVol         1024       //正弦波1周期のステップ数を指定:
#define triWaveFreq     12         //搬送波を正弦波の何倍の周波数にするかを指定、必ず4の倍数に:


float Tau = 2 * PI;

uint8_t uSin[waveVol];
uint8_t vSin[waveVol];
uint8_t wSin[waveVol];
uint8_t cTri[waveVol];

void setup() {
  Serial.begin(BaudRate);

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
}

void loop() {}
