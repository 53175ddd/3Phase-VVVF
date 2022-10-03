#define U_OUT D0
#define V_OUT D1
#define W_OUT D2

#define DEBUG false

// MODE true : 正弦波PWM, false : 三角波比較PWM
#define MODE  true

const float  Tau = PI * 2;  // τ = 2π:
const float qTau = PI / 2;  // Tauの1/4 (Quarter):

uint16_t amp   = 0;  // 波形の振幅:
uint16_t phase = 0;  // 出力波形の位相:
uint16_t times = 0;  // 遅延関数に渡す引数:
uint8_t  steps = 0;  // 出力ステップを保存:
uint16_t count = 1;

struct s_wave {      // 生成した波形の値の保存に使用:
  float u;    // U相正弦波:
  float v;    // V相正弦波:
  float w;    // W相正弦波:
  float t;    // 搬送三角波:
};

struct s_pwm {       // 出力する信号の保存に使用:
  uint8_t u;  // U相PWM出力値:
  uint8_t v;  // V相PWM出力値:
  uint8_t w;  // W相PWM出力値:
};

void setup() {
  Serial.begin(115200);

  pinMode(U_OUT, OUTPUT);
  pinMode(V_OUT, OUTPUT);
  pinMode(W_OUT, OUTPUT);

  times = 1500;
  amp   = 127;

  for(uint8_t i = 0; i < 128; i++) {
    char  buff[31];
    float dat = tri((i * Tau) / 128);
    sprintf(buff, "count [%3d] = %f", i, dat);
    Serial.println(buff);
  }

//  while(true);
}

void loop() {
  s_wave wave = {sin(((phase * Tau) / 128) + (1 * Tau / 3)),
                 sin(((phase * Tau) / 128) + (2 * Tau / 3)),
                 sin(((phase * Tau) / 128) + (3 * Tau / 3)),
                 tri ((phase * Tau) * 6 / 128)};

  if(MODE) {  // 正弦波信号をPWMで生成:
    s_pwm  pwms = {wave.u * amp + amp,
                   wave.v * amp + amp,
                   wave.w * amp + amp};
    if(phase > 127) {
      phase = 0;
    }else {
      phase++;
    }
  
    analogWrite(U_OUT, pwms.u);
    analogWrite(V_OUT, pwms.v);
    analogWrite(W_OUT, pwms.w);
  
    if(DEBUG) {  // 3相PWM数値出力:
      char buff[16];
      sprintf(buff, "%3d, %3d, %3d", pwms.u, pwms.v, pwms.w);
      Serial.println(buff);
    }
  }else {     // 三角波比較PWM:
    s_pwm pwms = {wave.u > wave.t ? 1 : 0,
                  wave.v > wave.t ? 1 : 0,
                  wave.w > wave.t ? 1 : 0};

    digitalWrite(U_OUT, pwms.u);
    digitalWrite(V_OUT, pwms.v);
    digitalWrite(W_OUT, pwms.w);

    if(phase > 127) {
      phase = 0;
    }else {
      phase++;
    }
  }

  delayMicroseconds(times);

  if(count == 180) {
    count = 1;
    if(times < 50) {
      times = 50;
    }else {
      times = times - 10;
    }
  }else {
    count++;
  }
}

float tri(const float phase) {
  if(0 <= phase && phase <= Tau) {    // 0 ~ Tauの範囲を指定された場合の処理ここから:
    if(phase <= qTau) {               //   0 ~ 1/4:
      float y = (phase * 2) / PI;
      return y;
    }else if(phase <= (qTau + PI)) {  // 1/4 ~ 3/4:
      float n = phase - qTau;
      float y = 1 - (n * 2) / PI;
      return y;
    }else {                           // 3/4 ~   1:
      float n = phase - (qTau + PI);
      float y = ((n * 2) / PI) - 1;
      return y;
    }
  }else{                              // 0 ~ Tauの範囲を指定された場合の処理ここまで:
    return tri(phase - Tau);          // 間違った範囲の引数が渡された場合は再起処理で対処:
  }
}
