// AlashAnalog: напряжение батареи через делитель. Аналоговый вход выдерживает не больше напряжения питания платы (5 В на Uno, 3.3 В на ESP32),
// поэтому батарею (например, 7-12 В или Li-ion до 4.2 В на ESP32) подают через делитель из двух резисторов:
//   батарея + - R1 - A0 - R2 - GND.   Коэффициент делителя = (R1 + R2) / R2. Здесь R1 = 10 кОм, R2 = 10 кОм: коэффициент 2.
#include <AlashAnalog.h>

AlashAnalog battery(A0);

void setup() {
  Serial.begin(115200);
  battery.setDivider(2.0);                 // (10 + 10) / 10
  // battery.setReference(4.95);           // если замерили мультиметром напряжение питания платы, укажите его: точность выше
}

void loop() {
  Serial.print(battery.voltage(), 2);
  Serial.println(F(" В"));
  delay(1000);
}
