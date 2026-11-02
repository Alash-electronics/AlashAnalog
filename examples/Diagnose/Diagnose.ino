// [ru] AlashAnalog: датчик показывает ерунду? Скетч читает пин 2 секунды и говорит: нет питания датчика, оборван сигнал или провод висит в воздухе.
// [kk] AlashAnalog: датчик бекер көрсетіп тұр ма? Скетч пинді 2 секунд оқиды және айтады: датчикте қуат жоқ, сигнал сымы үзілген немесе сым ауада ілулі.
// [en] AlashAnalog: the sensor shows nonsense? The sketch reads the pin for 2 seconds and says: no sensor power, a broken signal wire or a wire hanging in the air.
#include <AlashAnalog.h>

AlashAnalog sensor(A0);

void setup() {
  Serial.begin(115200);
  delay(500);
  sensor.setLanguage(AlashAnalog::LANG_RU);   // LANG_KK: қазақша, LANG_EN: English
  sensor.diagnose(Serial);
}

void loop() {}
