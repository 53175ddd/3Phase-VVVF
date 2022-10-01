#define U_OUT D0
#define V_OUT D1
#define W_OUT D2

#define delayTime 200

#define DEBUG true

const float  Tau = PI * 2;  // τ = 2π:
const float qTau = PI / 2;  // Tauの1/4 (Quarter):

uint16_t amp   = 0;  // 波形の振幅:
uint16_t phase = 0;  // 出力波形の位相:

struct s_phases {    // 生成した波形の値の保存に使用:
  float u;    // U相正弦波:
  float v;    // V相正弦波:
  float w;    // W相正弦波:
  float t;    // 搬送三角波:
};

struct s_pwms {      // 出力する信号の保存に使用:
  uint8_t u;  // U相PWM出力値:
  uint8_t v;  // V相PWM出力値:
  uint8_t w;  // W相PWM出力値:
};

void setup() {
  Serial.begin(115200);

  pinMode(U_OUT, OUTPUT);
  pinMode(V_OUT, OUTPUT);
  pinMode(W_OUT, OUTPUT);

  amp = 120;
}

void loop() {
  s_pwms   wave = {sin(((phase * Tau) / 128) + (1 * Tau / 3)) * amp + amp,
                   sin(((phase * Tau) / 128) + (2 * Tau / 3)) * amp + amp,
                   sin(((phase * Tau) / 128) + (3 * Tau / 3)) * amp + amp};
  if(phase > 127) {
    phase = 0;
  }else {
    phase++;
  }

  analogWrite(U_OUT, wave.u);
  analogWrite(V_OUT, wave.v);
  analogWrite(W_OUT, wave.w);

  if(DEBUG) {  // 3相PWM数値出力:
    char buff[16];
    sprintf(buff, "%3d, %3d, %3d", wave.u, wave.v, wave.w);
    Serial.println(buff);
  }

  delayMicroseconds(delayTime);
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
