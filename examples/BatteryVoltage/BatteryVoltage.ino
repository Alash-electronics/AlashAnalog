// [ru] AlashAnalog: напряжение батареи через делитель. Аналоговый вход выдерживает не больше напряжения питания платы (5 В на Uno, 3.3 В на ESP32),
//      поэтому батарею (например, 7-12 В или Li-ion до 4.2 В на ESP32) подают через делитель из двух резисторов:
//        батарея + - R1 - A0 - R2 - GND.   Коэффициент делителя = (R1 + R2) / R2. Здесь R1 = 10 кОм, R2 = 10 кОм: коэффициент 2.
// [kk] AlashAnalog: батарея кернеуі бөлгіш арқылы. Аналогтық кіріс тақта қуат кернеуінен артығын көтермейді (Uno-да 5 В, ESP32-де 3.3 В),
//      сондықтан батареяны (мысалы, 7-12 В немесе ESP32-де 4.2 В-қа дейінгі Li-ion) екі резисторлы бөлгіш арқылы қосады:
//        батарея + - R1 - A0 - R2 - GND.   Бөлгіш коэффициенті = (R1 + R2) / R2. Мұнда R1 = 10 кОм, R2 = 10 кОм: коэффициент 2.
// [en] AlashAnalog: battery voltage through a divider. An analog input tolerates no more than the board supply voltage (5 V on Uno, 3.3 V on ESP32),
//      so a battery (for example 7-12 V, or Li-ion up to 4.2 V on ESP32) is connected through a divider of two resistors:
//        battery + - R1 - A0 - R2 - GND.   Divider ratio = (R1 + R2) / R2. Here R1 = 10 kOhm, R2 = 10 kOhm: ratio 2.
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
