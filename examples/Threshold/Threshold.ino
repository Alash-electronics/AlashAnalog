// AlashAnalog: автополив с гистерезисом. Насос (через реле на пине 7, активный LOW) включается, когда почва высохла,
// и выключается, когда увлажнилась. Два порога не дают насосу дёргаться на границе.
// Подставьте в setRange свои значения из примера SoilMoisture.
#include <AlashAnalog.h>

AlashAnalog soil(A0, AlashAnalog::SOIL);
const uint8_t PUMP = 7;

void setup() {
  Serial.begin(115200);
  digitalWrite(PUMP, HIGH);                // реле выключено (активный LOW), сначала уровень, потом OUTPUT
  pinMode(PUMP, OUTPUT);
  soil.setRange(800, 300);                 // сухо = 800, вода = 300 (подберите калибровкой)
  soil.setThreshold(30, 60);               // "высокий" (влажно) выше 60 %, "низкий" (сухо) ниже 30 %
}

void loop() {
  bool wet = soil.isHigh();                // true, пока влажность выше порога; между 30 и 60 % держит прежнее состояние
  digitalWrite(PUMP, wet ? HIGH : LOW);    // сухо: насос включён (LOW), влажно: выключен
  if (soil.changed()) Serial.println(wet ? F("Влажно: насос выключен") : F("Сухо: включаю насос"));
  delay(1000);
}
