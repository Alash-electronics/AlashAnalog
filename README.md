# AlashAnalog

[Русский](#русский) | [Қазақша](#қазақша) | [English](#english)

---

## Русский

Простая библиотека, которая превращает сырые числа `analogRead()` в понятное: **проценты** по калибровке, **слова** («сухо, пора поливать», «темно», «газ! проветрите»), **напряжение** батареи через делитель и **пороги с гистерезисом** для автополива и освещения. Для датчиков влажности почвы, фоторезисторов, уровня воды, MQ-газа, микрофонов, потенциометров. Без зависимостей. Сделана для уроков и наборов Alash Electronics.

```cpp
#include <AlashAnalog.h>
AlashAnalog soil(A0, AlashAnalog::SOIL);

void setup() {
  Serial.begin(115200);
  soil.setRange(800, 300);          // сухо = 800, вода = 300 / dry = 800, water = 300
}
void loop() {
  Serial.println(soil.percent());   // 0..100 %
  Serial.println(soil.describe());  // "сухо, пора поливать" / "dry, time to water"
  delay(1000);
}
```

### Что умеет
- **Проценты:** `percent()` по двум точкам калибровки (`setRange(сухо, мокро)`), шкала сама обращается, если у датчика больше число значит суше.
- **Автокалибровка:** `autoRange(10000)`: за 10 секунд двигайте датчик между крайними состояниями, границы найдутся сами; `printCalibration()` печатает строку для вставки.
- **Без выбросов:** медиана по нескольким измерениям, `setSmoothing()`.
- **Слова:** `describe()` для почвы, света, воды, газа и звука на русском, казахском, английском.
- **Напряжение:** `voltage()` с учётом питания АЦП и делителя `setDivider(2.0)`: батареи, блоки питания.
- **Пороги с гистерезисом:** `setThreshold(30, 60)`, `isHigh()` и `changed()`: насос или лампа не дрожат на границе.
- **Самопроверка:** `diagnose()` читает пин 2 секунды и говорит: нет питания датчика, оборван сигнал, провод висит; на ESP32 предупреждает про АЦП2 и Wi-Fi.
- Определяет шкалу АЦП сама: Uno 1023 и 5 В, ESP32 4095 и 3.3 В.
- Работает на **Arduino AVR, ESP32, ESP8266**.

### Подключение
| Датчик | Uno / Nano | ESP32 |
|---|---|---|
| VCC | 5 В (или 3.3 В) | 3.3 В |
| GND | GND | GND |
| сигнал (AO / S) | A0 | GPIO34 (АЦП1) |

На ESP32 используйте пины АЦП1 (GPIO32-39): пины АЦП2 не работают при включённом Wi-Fi. Аналоговый вход нельзя подавать больше напряжения питания платы: для 5-12 В нужен делитель.

### Чем отличается от других библиотек
Мы посмотрели примеры из курсов, Smoothed (Mark Fickett), RunningMedian (Rob Tillaart) и ResponsiveAnalogRead:
- Это фильтры; процент, слова, калибровка и пороги нужно писать самим через `map()` и `if`. У нас всё вместе.
- `map()` не ограничивает результат и не умеет обращённую шкалу без ухищрений; у нас зажим 0..100 и автоматическая инверсия.
- Нет самопроверки: непонятно, датчик сломан или не подключён; у нас `diagnose()`.

### Примеры
`SoilMoisture`, `LightSensor`, `Threshold`, `BatteryVoltage`, `Diagnose`

### Тесты
Проценты прямой и обращённой шкалы с зажимом, медиана против выбросов, напряжение с делителем, калибровка по двум состояниям, автокалибровка по качанию (и отказ, если не двигали), гистерезис порогов, слова по виду и языку, диагностика (залипание на нуле и максимуме, висящий провод) проверены без железа: `extras/test`. При тестировании найдена и исправлена ошибка: настройки до `begin()` затирались значениями по умолчанию.

Подробное руководство: [docs/WIKI.md](docs/WIKI.md).

---

## Қазақша

`analogRead()` шикі сандарын түсінікті етіп өзгертетін қарапайым кітапхана: калибрлеу бойынша **пайыз**, **сөздер** («құрғақ, суару керек», «қараңғы»), бөлгіш арқылы батарея **кернеуі** және автосуару мен жарықтандыруға арналған гистерезисті **шектер**. Топырақ ылғалдылығы, фоторезистор, су деңгейі, MQ газ датчиктеріне. Тәуелділіксіз.

```cpp
#include <AlashAnalog.h>
AlashAnalog soil(A0, AlashAnalog::SOIL);

void setup() {
  Serial.begin(115200);
  soil.setRange(800, 300);          // сухо = 800, вода = 300 / dry = 800, water = 300
}
void loop() {
  Serial.println(soil.percent());   // 0..100 %
  Serial.println(soil.describe());  // "сухо, пора поливать" / "dry, time to water"
  delay(1000);
}
```

### Мүмкіндіктері
- **Пайыз:** `percent()` екі нүктелі калибрлеу бойынша, шкала өзі айналады.
- **Авто-калибрлеу:** `autoRange(10000)`; `printCalibration()`.
- **Шығарындысыз:** бірнеше өлшемнің медианасы.
- **Сөздер:** `describe()` топырақ, жарық, су, газ, дыбыс үшін үш тілде.
- **Кернеу:** `voltage()` бөлгішпен.
- **Гистерезисті шектер:** `setThreshold(30, 60)`, `isHigh()`, `changed()`.
- **Өзін тексеру:** `diagnose()`: қуат жоқ, сым үзілген, сым ауада.
- АЦП шкаласын өзі анықтайды.
- **Arduino AVR, ESP32, ESP8266**.

### Қосу
| Сенсор | Uno / Nano | ESP32 |
|---|---|---|
| VCC | 5 В (или 3.3 В) | 3.3 В |
| GND | GND | GND |
| сигнал (AO / S) | A0 | GPIO34 (АЦП1) |

ESP32-де АЦП1 пиндерін (GPIO32-39) қолданыңыз. Аналог кірісіне тақта қуатынан артық кернеу бермеңіз.

### Басқа кітапханалардан айырмашылығы
Курс мысалдары, Smoothed, RunningMedian және ResponsiveAnalogRead қарадық:
- Олар тек сүзгі; пайыз, сөз, калибрлеу, шектер бізде бар.
- `map()` нәтижені шектемейді; бізде 0..100 қысу және инверсия.
- Бізде `diagnose()` бар.

### Мысалдар
`SoilMoisture`, `LightSensor`, `Threshold`, `BatteryVoltage`, `Diagnose`

### Тесттер
Тура және кері шкала, медиана, бөлгіш кернеуі, калибрлеу, авто-калибрлеу, гистерезис, сөздер, диагностика тексерілген: `extras/test`.

Толық нұсқаулық: [docs/WIKI.md](docs/WIKI.md).

---

## English

A simple library that turns raw `analogRead()` numbers into something useful: **percent** from a calibration, **words** ("dry, time to water", "dark", "gas! ventilate"), battery **voltage** through a divider and **hysteresis thresholds** for watering and lighting. For soil moisture sensors, photoresistors, water level, MQ gas sensors, microphones and potentiometers. No dependencies. Made for classes and Alash Electronics kits.

```cpp
#include <AlashAnalog.h>
AlashAnalog soil(A0, AlashAnalog::SOIL);

void setup() {
  Serial.begin(115200);
  soil.setRange(800, 300);          // сухо = 800, вода = 300 / dry = 800, water = 300
}
void loop() {
  Serial.println(soil.percent());   // 0..100 %
  Serial.println(soil.describe());  // "сухо, пора поливать" / "dry, time to water"
  delay(1000);
}
```

### Features
- **Percent:** `percent()` from a two-point calibration (`setRange(dry, wet)`); the scale inverts by itself when a bigger number means drier.
- **Auto calibration:** `autoRange(10000)`: move the sensor between its extremes for 10 seconds and the limits are found; `printCalibration()` prints a line to paste.
- **No spikes:** a median over several readings, `setSmoothing()`.
- **Words:** `describe()` for soil, light, water, gas and sound in Russian, Kazakh and English.
- **Voltage:** `voltage()` using the ADC reference and a divider `setDivider(2.0)`: batteries, power supplies.
- **Hysteresis thresholds:** `setThreshold(30, 60)`, `isHigh()` and `changed()`: a pump or lamp does not chatter at the boundary.
- **Self-check:** `diagnose()` reads the pin for 2 seconds and says: no sensor power, broken signal, floating wire; on ESP32 it warns about ADC2 and Wi-Fi.
- Knows the ADC scale: Uno 1023 and 5 V, ESP32 4095 and 3.3 V.
- Works on **Arduino AVR, ESP32, ESP8266**.

### Wiring
| Sensor | Uno / Nano | ESP32 |
|---|---|---|
| VCC | 5 В (или 3.3 В) | 3.3 В |
| GND | GND | GND |
| сигнал (AO / S) | A0 | GPIO34 (АЦП1) |

On ESP32 use ADC1 pins (GPIO32-39): ADC2 pins do not work while Wi-Fi is on. Never feed an analog input more than the board supply: use a divider for 5-12 V.

### How it differs from other libraries
We looked at course examples, Smoothed (Mark Fickett), RunningMedian (Rob Tillaart) and ResponsiveAnalogRead:
- Those are filters; percent, words, calibration and thresholds you write yourself with `map()` and `if`. We have it all together.
- `map()` does not clamp the result and has no inverted scale without tricks; we clamp to 0..100 and invert automatically.
- No self-check; we have `diagnose()`.

### Examples
`SoilMoisture`, `LightSensor`, `Threshold`, `BatteryVoltage`, `Diagnose`

### Tests
Percent on straight and inverted scales with clamping, median against spikes, voltage with a divider, two-state calibration, auto calibration by sweeping (and refusing when not moved), threshold hysteresis, words by kind and language and diagnostics (stuck at zero and max, floating wire) are checked without hardware: `extras/test`. Testing found and fixed a bug: settings made before `begin()` were overwritten by defaults.

Full guide: [docs/WIKI.md](docs/WIKI.md).

---

MIT License, Alash Electronics.
