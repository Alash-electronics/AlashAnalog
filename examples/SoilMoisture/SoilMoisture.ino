// [ru] AlashAnalog: влажность почвы в процентах и словами. Ёмкостный (или резистивный) датчик влажности на пине A0.
//
//      Калибровка (один раз): датчик сухой на воздухе даёт одно число, в стакане воды другое. Откройте монитор порта:
//      пока датчик сухой, отправьте любой символ, затем опустите в воду (до отметки) и отправьте ещё один.
//      Скетч напечатает строку setRange(...): вставьте её в setup() и калибровка больше не нужна.
// [kk] AlashAnalog: топырақ ылғалдылығы пайызбен және сөзбен. A0 пинінде сыйымдылықты (немесе резистивті) ылғалдылық датчигі.
//
//      Калибрлеу (бір рет): құрғақ датчик ауада бір сан береді, стақандағы суда басқа. Порт мониторын ашыңыз:
//      датчик құрғақ кезде кез келген таңба жіберіңіз, сосын суға салып (белгіге дейін) тағы біреуін жіберіңіз.
//      Скетч setRange(...) жолын басып шығарады: оны setup() ішіне қойыңыз, калибрлеу енді қажет емес.
// [en] AlashAnalog: soil moisture in percent and in words. A capacitive (or resistive) moisture sensor on pin A0.
//
//      Calibration (once): a dry sensor in air gives one number, in a glass of water another. Open the serial monitor:
//      while the sensor is dry, send any character, then put it in water (up to the mark) and send one more.
//      The sketch prints a setRange(...) line: paste it into setup() and no more calibration is needed.
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
