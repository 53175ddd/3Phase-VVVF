#define hoge hoge
const float Pi = 3.1416
uint8_t sinWave[720];
uint8_t triWave[360];
const uint8_t musk[8] = {0b10000000, 0b01000000, 0b00100000, 0b00010000, 0b00001000, 0b00000100, 0b00000010, 0b00000001};
const uint8_t pinMusk = 0b11111100;
      uint8_t Buffer  = 0b00000000;

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
  DDRD = pinMusk;
}

void loop() {
}

void fastShiftOut(uint8_t t, uint8_t u, uint8_t v, uint8_t w){
  //MSBから順にラッチクロック, クロック,　triWave, U, V, W:
  for(uint8_t i = 0; i < 8; ){
    Buffer = ((t & musk[i]) >> 2) | ((u & musk[i]) >> 3) | ((v & musk[i]) >> 4) | ((w & musk[i]) >> 5);
    PORTD  = Buffer;
    Buffer = Buffer | 0b01000000;
    PORTD  = Buffer & 0b00111111;
  }
}
