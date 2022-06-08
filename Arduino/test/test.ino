/*
 * VVVFインバータの動作テスト用プログラム
 * 矩形波駆動方式
 */

#define delayTime 10000

uint8_t gatesSignal[12] = {000110, 100110, 100100, 100101, 100001, 101001,
                           001001, 011001, 011000, 011010, 100010, 010110};

void setup() {
  //UH,UL,VH,VL,WH,WL:
  DDRC = 0b111111;
}

void loop() {
  for(uint8_t i = 0; i < sizeof(gatesSignal); i++) {
    PORTC = gatesSignal[i];
    delayMicroseconds(delayTime);
  }
}
