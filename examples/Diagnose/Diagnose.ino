// AlashAnalog: датчик показывает ерунду? Скетч читает пин 2 секунды и говорит: нет питания датчика, оборван сигнал или провод висит в воздухе.
#include <AlashAnalog.h>

AlashAnalog sensor(A0);

void setup() {
  Serial.begin(115200);
  delay(500);
  sensor.setLanguage(AlashAnalog::LANG_RU);   // LANG_KK: қазақша, LANG_EN: English
  sensor.diagnose(Serial);
}

void loop() {}
