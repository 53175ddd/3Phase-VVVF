const float  Tau = PI * 2;  // τ = 2π:
const float qTau = PI / 2;  // Tauの1/4 (Quarter)の意味:

struct s_phases {
  float u;  // U相正弦波:
  float v;  // V相正弦波:
  float w;  // W相正弦波:
  float t;  // 搬送三角波:
};

struct s_pwms {
  uint8_t u;  // U相PWM出力値:
  uint8_t v;  // V相PWM出力値:
  uint8_t w;  // W相PWM出力値:
};

void setup() {
  struct s_phases wave;      // 生成した波形の値の保存に使用:
  wave.u = 0;
  wave.v = 0;
  wave.w = 0;

  struct s_pwms digitalOut;  // 出力する信号の保存に使用:
  digitalOut.u = 0;
  digitalOut.v = 0;
  digitalOut.w = 0;
  
}

void loop() {
  
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
