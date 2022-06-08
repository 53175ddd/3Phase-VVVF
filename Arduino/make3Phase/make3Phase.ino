//8bit幅の三相正弦波をCSV形式で出力します:
#define BaudRate        115200
#define waveVol         1000

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
  for(uint16_t i = 0; i < 10; i++) {
    for(uint8_t o = 0; o < 50; o++) {
      uint16_t phase = o + 20 * i * 2;
      cTri[phase] = (255 / 50) * o;
    Serial.print(String(cTri[phase]) + ",");
    }
    for(uint8_t o = 50; o > 0; o--) {
      uint16_t phase = (o + 1) + 20 * i * 2;
      cTri[phase] = (255 / 50) * o;
    Serial.print(String(cTri[phase]) + ",");
    }
  }
}

void loop() {
  
}
