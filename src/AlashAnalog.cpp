// MIT License. Alash Electronics.
#include "AlashAnalog.h"

#define T(ru, kk, en) (_lang == LANG_KK ? F(kk) : _lang == LANG_EN ? F(en) : F(ru))

void AlashAnalog::begin() {
  _started = true;
  // Значения по умолчанию для платы; то, что пользователь задал сам (setAdcMax / setReference) до begin(), не трогаем
#if defined(ESP32) || defined(ARDUINO_ARCH_RP2040)
  if (!_adcSet) _adcMax = 4095;
  if (!_refSet) _vref = 3.3f;
#elif defined(ESP8266)
  if (!_adcSet) _adcMax = 1023;
  if (!_refSet) _vref = 3.3f;                 // на платах NodeMCU / Wemos стоит делитель: 0-3.3 В на входе A0 (у голого чипа было бы 0-1 В)
#else
  if (!_adcSet) _adcMax = 1023;
  if (!_refSet) _vref = 5.0f;
#endif
}

int AlashAnalog::raw() {
  if (!_started) begin();
  return analogRead(_pin);
}

// Медиана нескольких измерений: одиночные выбросы (наводки, шум АЦП) не проходят
int AlashAnalog::filtered() {
  if (!_started) begin();
  int v[15];
  for (uint8_t i = 0; i < _samples; i++) v[i] = analogRead(_pin);
  for (uint8_t i = 1; i < _samples; i++) {               // сортировка вставками
    int x = v[i], j = i - 1;
    while (j >= 0 && v[j] > x) { v[j + 1] = v[j]; j--; }
    v[j + 1] = x;
  }
  return v[_samples / 2];
}

int AlashAnalog::averageRaw(uint8_t samples) {
  if (!_started) begin();
  if (samples < 1) samples = 1;
  long sum = 0;
  for (uint8_t i = 0; i < samples; i++) {
    sum += filtered();
    delay(2);
  }
  return (int)(sum / samples);
}

float AlashAnalog::percent() {
  if (!_hasRange) {
    _err = ERR_NO_RANGE;
    return NAN;
  }
  float v = (float)filtered();
  float p = (v - _low) * 100.0f / (float)(_high - _low);  // при low > high шкала обращается сама
  if (p < 0) p = 0;
  if (p > 100) p = 100;
  _err = ERR_OK;
  updateThreshold(p);
  return p;
}

float AlashAnalog::voltage() {
  return filtered() * _vref / (float)_adcMax * _divider;
}

// Автокалибровка: за durationMs находим самое малое и самое большое значение. Датчик за это время надо поместить в оба крайних состояния.
bool AlashAnalog::autoRange(uint16_t durationMs) {
  if (!_started) begin();
  int mn = 0x7FFF, mx = -1;
  uint32_t t0 = millis();
  while ((uint32_t)(millis() - t0) < durationMs) {
    int v = filtered();
    if (v < mn) mn = v;
    if (v > mx) mx = v;
    delay(10);
  }
  if (mx - mn < _adcMax / 20) {                           // размах меньше 5 % шкалы: датчик не двигали
    _err = ERR_NO_RANGE;
    return false;
  }
  _low = mn;
  _high = mx;
  _hasRange = true;
  _err = ERR_OK;
  return true;
}

void AlashAnalog::printCalibration(Print &out) {
  out.print(F("sensor.setRange("));
  out.print(_low);
  out.print(F(", "));
  out.print(_high);
  out.println(F(");"));
}

// ---- Пороги --------------------------------------------------------------------------------

void AlashAnalog::setThreshold(float lowPercent, float highPercent) {
  _thLow = lowPercent;
  _thHigh = highPercent;
  _thSet = true;
}

void AlashAnalog::updateThreshold(float pct) {
  if (!_thSet) return;
  bool was = _thOn;
  if (!_thOn && pct >= _thHigh) _thOn = true;
  else if (_thOn && pct <= _thLow) _thOn = false;
  if (_thOn != was) _thChanged = true;
}

bool AlashAnalog::isHigh() {
  percent();
  return _thOn;
}

bool AlashAnalog::changed() {
  percent();
  bool c = _thChanged;
  _thChanged = false;
  return c;
}

// ---- Слова -----------------------------------------------------------------------------------

const __FlashStringHelper *AlashAnalog::describe() {
  float p = percent();
  if (isnan(p)) return F("?");
  switch (_kind) {
    case SOIL:
      if (p < 25) return T("сухо, пора поливать", "құрғақ, суару керек", "dry, time to water");
      if (p < 60) return T("нормально", "қалыпты", "normal");
      if (p < 85) return T("влажно", "ылғалды", "moist");
      return T("очень мокро", "өте дымқыл", "soaking wet");
    case LIGHT:
      if (p < 10) return T("темно", "қараңғы", "dark");
      if (p < 40) return T("сумерки", "ымырт", "dim");
      if (p < 75) return T("светло", "жарық", "bright");
      return T("очень ярко", "өте жарық", "very bright");
    case WATER:
      if (p < 10) return T("пусто", "бос", "empty");
      if (p < 50) return T("мало воды", "су аз", "low");
      if (p < 90) return T("достаточно", "жеткілікті", "enough");
      return T("полно", "толы", "full");
    case GAS:
      if (p < 20) return T("воздух чистый", "ауа таза", "clean air");
      if (p < 50) return T("есть примеси", "қоспа бар", "some gas");
      return T("газ! проветрите", "газ! желдетіңіз", "gas! ventilate");
    case SOUND:
      if (p < 15) return T("тихо", "тыныш", "quiet");
      if (p < 50) return T("шумно", "шулы", "noisy");
      return T("очень громко", "өте қатты", "very loud");
    default:
      return F("");
  }
}

// ---- Диагностика ------------------------------------------------------------------------------

const __FlashStringHelper *AlashAnalog::errorText() {
  switch (_err) {
    case ERR_OK: return F("OK");
    case ERR_STUCK_LOW:
      return T("Значение всегда около нуля: проверьте питание датчика (VCC и GND) и что сигнальный провод не замкнут на землю",
               "Мән әрқашан нөлге жақын: сенсор қуатын (VCC және GND) және сигнал сымы жерге тұйықталмағанын тексеріңіз",
               "The value is always near zero: check the sensor power (VCC and GND) and that the signal wire is not shorted to ground");
    case ERR_STUCK_HIGH:
      return T("Значение всегда около максимума: сигнальный провод оборван или замкнут на питание",
               "Мән әрқашан максимумға жақын: сигнал сымы үзілген не қуатқа тұйықталған",
               "The value is always near the maximum: the signal wire is broken or shorted to power");
    case ERR_FLOATING:
      return T("Значение шумит само по себе: сигнальный провод никуда не подключён (висит в воздухе). Подключите его к выходу датчика",
               "Мән өздігінен шулайды: сигнал сымы ешқайда қосылмаған. Оны сенсор шығысына қосыңыз",
               "The value is noisy by itself: the signal wire is not connected (floating). Connect it to the sensor output");
    case ERR_NO_RANGE:
      return T("Диапазон не задан: вызовите setRange(сухо, мокро) или autoRange() и двигайте датчик между крайними состояниями",
               "Ауқым берілмеген: setRange(құрғақ, дымқыл) не autoRange() шақырып, сенсорды шеткі күйлер арасында жылжытыңыз",
               "No range set: call setRange(dry, wet) or autoRange() and move the sensor between its extreme states");
  }
  return F("");
}

void AlashAnalog::diagnose(Print &out) {
  if (!_started) begin();
  out.println(T("AlashAnalog: диагностика (читаю пин 2 секунды)", "AlashAnalog: диагностика (пинді 2 секунд оқимын)", "AlashAnalog: diagnostics (reading the pin for 2 seconds)"));
  out.print(T("Пин: ", "Пин: ", "Pin: "));
  out.println((int)_pin);
  int mn = 0x7FFF, mx = -1;
  long sum = 0;
  uint16_t n = 0;
  uint32_t t0 = millis();
  while ((uint32_t)(millis() - t0) < 2000) {
    int v = analogRead(_pin);
    if (v < mn) mn = v;
    if (v > mx) mx = v;
    sum += v;
    n++;
    delay(5);
  }
  int mean = n ? (int)(sum / n) : 0;
  out.print(T("Минимум / среднее / максимум: ", "Минимум / орташа / максимум: ", "Min / mean / max: "));
  out.print(mn); out.print(F(" / ")); out.print(mean); out.print(F(" / ")); out.println(mx);
  out.print(T("Шкала АЦП: 0...", "АЦП шкаласы: 0...", "ADC scale: 0..."));
  out.println(_adcMax);
  if (mx - mn > _adcMax / 4) _err = ERR_FLOATING;               // сам по себе гуляет на четверть шкалы
  else if (mean < _adcMax / 100) _err = ERR_STUCK_LOW;
  else if (mean > _adcMax - _adcMax / 100) _err = ERR_STUCK_HIGH;
  else _err = ERR_OK;
  if (_err == ERR_OK) {
    out.println(T("Значения в норме. Двигайте датчик (сухо / мокро, темно / светло): число должно заметно меняться.",
                  "Мәндер қалыпты. Сенсорды жылжытыңыз: сан айтарлықтай өзгеруі керек.",
                  "The values look fine. Move the sensor (dry / wet, dark / light): the number must change noticeably."));
  } else {
    out.println(errorText());
  }
#if defined(ESP32)
  out.println(T("ESP32: пины АЦП2 (GPIO0, 2, 4, 12-15, 25-27) не работают, пока включён Wi-Fi. Используйте пины АЦП1: GPIO32-39.",
                "ESP32: Wi-Fi қосулы кезде АЦП2 пиндері (GPIO0, 2, 4, 12-15, 25-27) жұмыс істемейді. АЦП1 пиндерін қолданыңыз: GPIO32-39.",
                "ESP32: ADC2 pins (GPIO0, 2, 4, 12-15, 25-27) do not work while Wi-Fi is on. Use ADC1 pins: GPIO32-39."));
#endif
}
