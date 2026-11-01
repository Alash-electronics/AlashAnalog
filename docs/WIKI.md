# AlashAnalog: руководство / нұсқаулық / guide

[Русский](#русский) | [Қазақша](#қазақша) | [English](#english)

---

## Русский

Простая библиотека, которая превращает сырые числа `analogRead()` в понятное: **проценты** по калибровке, **слова** («сухо, пора поливать», «темно», «газ! проветрите»), **напряжение** батареи через делитель и **пороги с гистерезисом** для автополива и освещения. Для датчиков влажности почвы, фоторезисторов, уровня воды, MQ-газа, микрофонов, потенциометров. Без зависимостей. Сделана для уроков и наборов Alash Electronics.

### Установка
Arduino IDE: Скетч, Подключить библиотеку, Управлять библиотеками, найти **AlashAnalog** и нажать Установить. Или скачайте ZIP с GitHub: Скетч, Подключить библиотеку, Добавить .ZIP библиотеку.

### Платы
Arduino Uno / Nano / Mega (AVR), ESP32, ESP8266 и другие, где работает Arduino-ядро.

### Подключение
| Датчик | Uno / Nano | ESP32 |
|---|---|---|
| VCC | 5 В (или 3.3 В) | 3.3 В |
| GND | GND | GND |
| сигнал (AO / S) | A0 | GPIO34 (АЦП1) |

На ESP32 используйте пины АЦП1 (GPIO32-39): пины АЦП2 не работают при включённом Wi-Fi. Аналоговый вход нельзя подавать больше напряжения питания платы: для 5-12 В нужен делитель.

### Быстрый старт
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

### Справочник по методам
| Метод | Что делает |
|---|---|
| `AlashAnalog(pin, kind)` | Датчик на пине с видом SOIL, LIGHT, WATER, GAS, SOUND |
| `percent()` | 0..100 % по калибровке |
| `describe()` | Словами |
| `voltage()` | Напряжение, В (с делителем) |
| `filtered() / raw()` | Медиана / сырое значение |
| `setRange(low, high)` | Калибровка: значения для 0 % и 100 % |
| `calibrateLow() / calibrateHigh() / autoRange(ms)` | Калибровка по состояниям / автоматически |
| `setThreshold(low, high) / isHigh() / changed()` | Порог с гистерезисом |
| `setDivider(k) / setReference(v) / setAdcMax(n)` | Делитель, питание АЦП, шкала |
| `diagnose(Serial)` | Самопроверка пина |

### Если что-то не работает
**Проценты всегда 0 или 100**

Калибровка не соответствует датчику. Задайте `setRange()` по своим значениям (запустите Diagnose или SoilMoisture) или `autoRange()`.

**На ESP32 значения не читаются при включённом Wi-Fi**

Вы используете пин АЦП2. Переключитесь на пины АЦП1 (GPIO32-39).

**Датчик почвы быстро портится**

Резистивные датчики (две вилки) корродируют. Используйте ёмкостные датчики или подавайте питание на датчик только на время измерения.

### Самопроверка
В библиотеке есть `diagnose(Serial)`: она печатает, что видит плата, и подсказывает, что проверить. Язык подсказок: `setLanguage(LANG_RU / LANG_KK / LANG_EN)`.

### Примеры
`SoilMoisture`, `LightSensor`, `Threshold`, `BatteryVoltage`, `Diagnose`

---

## Қазақша

`analogRead()` шикі сандарын түсінікті етіп өзгертетін қарапайым кітапхана: калибрлеу бойынша **пайыз**, **сөздер** («құрғақ, суару керек», «қараңғы»), бөлгіш арқылы батарея **кернеуі** және автосуару мен жарықтандыруға арналған гистерезисті **шектер**. Топырақ ылғалдылығы, фоторезистор, су деңгейі, MQ газ датчиктеріне. Тәуелділіксіз.

### Орнату
Arduino IDE: Sketch, Include Library, Manage Libraries, **AlashAnalog** табып, Install басыңыз. Немесе GitHub-тан ZIP жүктеп, Sketch, Include Library, Add .ZIP Library.

### Тақталар
Arduino Uno / Nano / Mega (AVR), ESP32, ESP8266 және Arduino ядросы бар басқалар.

### Қосу
| Сенсор | Uno / Nano | ESP32 |
|---|---|---|
| VCC | 5 В (или 3.3 В) | 3.3 В |
| GND | GND | GND |
| сигнал (AO / S) | A0 | GPIO34 (АЦП1) |

ESP32-де АЦП1 пиндерін (GPIO32-39) қолданыңыз. Аналог кірісіне тақта қуатынан артық кернеу бермеңіз.

### Жылдам бастау
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

### Әдістер анықтамасы
| Әдіс | Не істейді |
|---|---|
| `AlashAnalog(pin, kind)` | Пиндегі сенсор |
| `percent()` | Калибрлеу бойынша 0..100 % |
| `describe()` | Сөзбен |
| `voltage()` | Кернеу, В |
| `filtered() / raw()` | Медиана / шикі мән |
| `setRange(low, high)` | Калибрлеу |
| `calibrateLow() / calibrateHigh() / autoRange(ms)` | Күйлер бойынша / автоматты |
| `setThreshold(low, high) / isHigh() / changed()` | Гистерезисті шек |
| `setDivider(k) / setReference(v) / setAdcMax(n)` | Бөлгіш, АЦП қуаты, шкала |
| `diagnose(Serial)` | Пинді тексеру |

### Бірдеңе жұмыс істемесе
**Пайыз әрқашан 0 не 100**

Калибрлеу сенсорға сәйкес емес. `setRange()` не `autoRange()` қолданыңыз.

**ESP32-де Wi-Fi қосулы кезде мәндер оқылмайды**

АЦП2 пинін қолданудасыз. АЦП1 пиндеріне ауысыңыз.

**Топырақ сенсоры тез бұзылады**

Резистивті сенсорлар тот басады. Сыйымдылықты сенсорларды қолданыңыз.

### Өзін-өзі тексеру
Кітапханада `diagnose(Serial)` бар: тақта не көргенін басып шығарады және нені тексеру керегін айтады. Кеңес тілі: `setLanguage(LANG_RU / LANG_KK / LANG_EN)`.

### Мысалдар
`SoilMoisture`, `LightSensor`, `Threshold`, `BatteryVoltage`, `Diagnose`

---

## English

A simple library that turns raw `analogRead()` numbers into something useful: **percent** from a calibration, **words** ("dry, time to water", "dark", "gas! ventilate"), battery **voltage** through a divider and **hysteresis thresholds** for watering and lighting. For soil moisture sensors, photoresistors, water level, MQ gas sensors, microphones and potentiometers. No dependencies. Made for classes and Alash Electronics kits.

### Installation
Arduino IDE: Sketch, Include Library, Manage Libraries, search **AlashAnalog** and click Install. Or download the ZIP from GitHub: Sketch, Include Library, Add .ZIP Library.

### Boards
Arduino Uno / Nano / Mega (AVR), ESP32, ESP8266 and others with an Arduino core.

### Wiring
| Sensor | Uno / Nano | ESP32 |
|---|---|---|
| VCC | 5 В (или 3.3 В) | 3.3 В |
| GND | GND | GND |
| сигнал (AO / S) | A0 | GPIO34 (АЦП1) |

On ESP32 use ADC1 pins (GPIO32-39): ADC2 pins do not work while Wi-Fi is on. Never feed an analog input more than the board supply: use a divider for 5-12 V.

### Quick start
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

### Method reference
| Method | What it does |
|---|---|
| `AlashAnalog(pin, kind)` | A sensor on a pin with a kind |
| `percent()` | 0..100 % from calibration |
| `describe()` | In words |
| `voltage()` | Voltage, V (with a divider) |
| `filtered() / raw()` | Median / raw value |
| `setRange(low, high)` | Calibration: values for 0 % and 100 % |
| `calibrateLow() / calibrateHigh() / autoRange(ms)` | By states / automatic |
| `setThreshold(low, high) / isHigh() / changed()` | Hysteresis threshold |
| `setDivider(k) / setReference(v) / setAdcMax(n)` | Divider, ADC reference, scale |
| `diagnose(Serial)` | Pin self-check |

### If something does not work
**Percent is always 0 or 100**

The calibration does not match the sensor. Set `setRange()` from your values (run Diagnose or SoilMoisture) or use `autoRange()`.

**On ESP32 values are unreadable with Wi-Fi on**

You are on an ADC2 pin. Switch to ADC1 pins (GPIO32-39).

**The soil sensor corrodes quickly**

Resistive sensors (two prongs) corrode. Use capacitive sensors or power the sensor only during the measurement.

### Self-check
The library has `diagnose(Serial)`: it prints what the board sees and tells what to check. Tips language: `setLanguage(LANG_RU / LANG_KK / LANG_EN)`.

### Examples
`SoilMoisture`, `LightSensor`, `Threshold`, `BatteryVoltage`, `Diagnose`

---

MIT License, Alash Electronics. https://github.com/Alash-electronics/AlashAnalog
