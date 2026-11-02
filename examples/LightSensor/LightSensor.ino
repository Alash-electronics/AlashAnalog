// [ru] AlashAnalog: фоторезистор (LDR) в делителе напряжения с резистором 10 кОм. Освещённость в процентах и словами.
//      Схема: 5 В - фоторезистор - A0 - резистор 10 кОм - GND (у такой схемы больше света даёт большее число).
//      autoRange() за 10 секунд сам находит границы: за это время закройте датчик ладонью и посветите на него телефоном.
// [kk] AlashAnalog: 10 кОм резисторы бар кернеу бөлгіштегі фоторезистор (LDR). Жарықтық пайызбен және сөзбен.
//      Сұлба: 5 В - фоторезистор - A0 - 10 кОм резистор - GND (мұндай сұлбада жарық көп болса сан үлкен болады).
//      autoRange() 10 секундта шекараларды өзі табады: осы уақытта датчикті алақанмен жауып, телефонмен жарық түсіріңіз.
// [en] AlashAnalog: a photoresistor (LDR) in a voltage divider with a 10 kOhm resistor. The light level in percent and in words.
//      Circuit: 5 V - photoresistor - A0 - 10 kOhm resistor - GND (in this circuit more light gives a larger number).
//      autoRange() finds the limits by itself in 10 seconds: during that time cover the sensor with your palm and shine a phone at it.
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
