#define hoge hoge
const float Pi = 3.1416
uint8_t sinWave[720];
uint8_t triWave[360];
uint8_t Buffer = 0b00000000;

void setup() {
  for(uint16_t i = 0; i < 360; i++){
    sinWave[i] = (128 * sin(Pi / (180 * i) + 1)) + 128;
    sinWave[i + 360] = sinWave[i];
    if(i < 90){
      triWave[i] = (128 * (90 / i));
    }else if(i < 270){
      triWave[i] = (128 * (90 / (270 - i)) + 128);
    }else{
      triWave[i] = (128 * (90 / (i - 270)) + 128);
    }
  }
}

void loop() {
}

void fastShiftOut(uint8_t dat){
  //MSBをラッチクロック, MSB-1をクロックとして使用,　残りは3相の波形:
  
}
