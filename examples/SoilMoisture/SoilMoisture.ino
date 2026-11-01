// AlashAnalog: влажность почвы в процентах и словами. Ёмкостный (или резистивный) датчик влажности на пине A0.
//
// Калибровка (один раз): датчик сухой на воздухе даёт одно число, в стакане воды другое. Откройте монитор порта:
// пока датчик сухой, отправьте любой символ, затем опустите в воду (до отметки) и отправьте ещё один.
// Скетч напечатает строку setRange(...): вставьте её в setup() и калибровка больше не нужна.
#include <AlashAnalog.h>

AlashAnalog soil(A0, AlashAnalog::SOIL);

void waitKey() {
  while (!Serial.available()) delay(10);
  while (Serial.available()) Serial.read();
}

void setup() {
  Serial.begin(115200);
  Serial.println(F("Датчик сухой, на воздухе. Отправьте любой символ..."));
  waitKey();
  soil.calibrateLow();                     // это 0 %
  Serial.println(F("Теперь опустите в воду и отправьте символ..."));
  waitKey();
  soil.calibrateHigh();                    // это 100 %
  soil.printCalibration(Serial);           // sensor.setRange(сухо, мокро): вставьте в setup() вместо калибровки
}

void loop() {
  Serial.print(soil.percent(), 0);
  Serial.print(F(" %  "));
  Serial.println(soil.describe());         // "сухо, пора поливать", "нормально", "влажно"
  delay(1000);
}
