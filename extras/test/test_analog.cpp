// Тесты AlashAnalog без железа (значения АЦП задаёт тест):
//   g++ -std=c++17 -Wall -I mock -I ../../src test_analog.cpp ../../src/AlashAnalog.cpp && ./a.out
#include <stdio.h>
#include "Arduino.h"
#include "AlashAnalog.h"

static int fails = 0;
#define CHECK(cond) do { if (!(cond)) { printf("FAIL line %d: %s\n", __LINE__, #cond); fails++; } } while (0)
static bool near(float a, float b, float e) { return fabsf(a - b) <= e; }
static int level = 0;
static int fixed() { return level; }
static int spiky() { static int n = 0; n++; return (n % 5 == 0) ? 1023 : level; }          // каждое пятое чтение выброс
static int sweep() { return 500 + (int)(400.0f * sinf((float)g_ms * 0.003f)); }              // качается 100..900
static int noisy() { static unsigned s = 12345; s = s * 1103515245u + 12345u; return (s >> 16) % 1024; }
static void reset() { g_ms = 1000; g_adc = fixed; level = 0; }

int main() {
  // 1) проценты: обычная и обращённая шкала (датчик почвы: сухой выдаёт больше), зажим по краям
  reset(); AlashAnalog a(0, AlashAnalog::LIGHT);
  CHECK(isnan(a.percent()) && a.error() == AlashAnalog::ERR_NO_RANGE);
  a.setRange(100, 900);
  level = 100; CHECK(near(a.percent(), 0, 0.01)); level = 900; CHECK(near(a.percent(), 100, 0.01)); level = 500; CHECK(near(a.percent(), 50, 0.01));
  level = 20; CHECK(a.percent() == 0); level = 1000; CHECK(a.percent() == 100);
  AlashAnalog s(0, AlashAnalog::SOIL); s.setRange(800, 300);                      // сухо 800 = 0 %, вода 300 = 100 %
  level = 800; CHECK(near(s.percent(), 0, 0.01)); level = 300; CHECK(near(s.percent(), 100, 0.01)); level = 550; CHECK(near(s.percent(), 50, 0.01));
  level = 900; CHECK(s.percent() == 0); level = 100; CHECK(s.percent() == 100);

  // 2) медиана убирает одиночные выбросы
  reset(); AlashAnalog m(0); level = 500; g_adc = spiky;
  for (int i = 0; i < 20; i++) CHECK(m.filtered() == 500);
  m.setSmoothing(1); bool sawSpike = false; for (int i = 0; i < 20; i++) if (m.filtered() == 1023) sawSpike = true;
  CHECK(sawSpike);                                                                 // без сглаживания выброс виден

  // 3) напряжение: Uno 5 В / 1023, делитель, ESP32 3.3 В / 4095
  reset(); AlashAnalog v(0); level = 512;
  CHECK(near(v.voltage(), 512 * 5.0f / 1023, 0.001f));
  v.setDivider(3.0f); CHECK(near(v.voltage(), 512 * 5.0f / 1023 * 3.0f, 0.005f));
  AlashAnalog e(0); e.setAdcMax(4095); e.setReference(3.3f); level = 4095; CHECK(near(e.voltage(), 3.3f, 0.001f));

  // 4) калибровка по двум состояниям
  reset(); AlashAnalog c(0); level = 790; c.calibrateLow(10); level = 310; c.calibrateHigh(10);
  CHECK(c.lowValue() == 790 && c.highValue() == 310);
  level = 550; CHECK(near(c.percent(), 50, 0.5));
  printf("--- calibration line ---\n"); c.printCalibration(*(new Print));

  // 5) автокалибровка по качанию и отказ, если не двигали
  reset(); AlashAnalog r(0); g_adc = sweep;
  CHECK(r.autoRange(10000));
  CHECK(r.lowValue() < 130 && r.highValue() > 870);
  reset(); AlashAnalog st(0); level = 400; CHECK(!st.autoRange(2000) && st.error() == AlashAnalog::ERR_NO_RANGE);

  // 6) порог с гистерезисом: включается выше 60 %, выключается ниже 40 %, в середине без дрожания
  reset(); AlashAnalog t(0); t.setRange(0, 1000); t.setThreshold(40, 60);
  level = 300; CHECK(!t.isHigh() && !t.changed());
  level = 650; CHECK(t.isHigh() && t.changed() && !t.changed());
  level = 500; CHECK(t.isHigh() && !t.changed());                                  // между порогами состояние не меняется
  level = 450; CHECK(t.isHigh());
  level = 350; CHECK(!t.isHigh() && t.changed());
  level = 500; CHECK(!t.isHigh());

  // 7) слова по виду датчика и языку
  reset(); AlashAnalog w(0, AlashAnalog::SOIL); w.setRange(800, 300);
  level = 750; CHECK(strcmp((const char *)w.describe(), "сухо, пора поливать") == 0);
  level = 400; w.setLanguage(AlashAnalog::LANG_EN); CHECK(strcmp((const char *)w.describe(), "moist") == 0 || strcmp((const char *)w.describe(), "normal") == 0);
  level = 320; CHECK(strcmp((const char *)w.describe(), "soaking wet") == 0);
  AlashAnalog g(0, AlashAnalog::GAS); g.setRange(100, 700); level = 650; g.setLanguage(AlashAnalog::LANG_KK); CHECK(strcmp((const char *)g.describe(), "газ! желдетіңіз") == 0);
  AlashAnalog ge(0); ge.setRange(0, 100); level = 50; CHECK(strcmp((const char *)ge.describe(), "") == 0);

  // 8) диагностика: нормальный сигнал, залипание на нуле и на максимуме, висящий провод
  reset(); AlashAnalog d(0); level = 512; d.diagnose(*(new Print)); CHECK(d.error() == AlashAnalog::ERR_OK);
  reset(); AlashAnalog d0(0); level = 1; d0.diagnose(*(new Print)); CHECK(d0.error() == AlashAnalog::ERR_STUCK_LOW);
  reset(); AlashAnalog d1(0); level = 1023; d1.diagnose(*(new Print)); CHECK(d1.error() == AlashAnalog::ERR_STUCK_HIGH);
  reset(); AlashAnalog df(0); g_adc = noisy; df.diagnose(*(new Print)); CHECK(df.error() == AlashAnalog::ERR_FLOATING);
  d.setLanguage(AlashAnalog::LANG_KK); CHECK(strlen((const char *)d.errorText()) > 0);

  if (fails) { printf("FAILED: %d\n", fails); return 1; }
  printf("ALL TESTS PASSED\n");
  return 0;
}
