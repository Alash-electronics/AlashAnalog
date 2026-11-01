// AlashAnalog: фоторезистор (LDR) в делителе напряжения с резистором 10 кОм. Освещённость в процентах и словами.
// Схема: 5 В - фоторезистор - A0 - резистор 10 кОм - GND (у такой схемы больше света даёт большее число).
// autoRange() за 10 секунд сам находит границы: за это время закройте датчик ладонью и посветите на него телефоном.
#include <AlashAnalog.h>

AlashAnalog light(A0, AlashAnalog::LIGHT);

void setup() {
  Serial.begin(115200);
  Serial.println(F("Калибровка 10 секунд: закройте датчик, потом посветите на него..."));
  if (!light.autoRange(10000)) {
    Serial.println(light.errorText());     // например, если датчик не двигали
    light.setRange(50, 900);               // запасные границы
  }
}

void loop() {
  Serial.print(light.percent(), 0);
  Serial.print(F(" %  "));
  Serial.println(light.describe());        // "темно", "сумерки", "светло", "очень ярко"
  delay(500);
}
