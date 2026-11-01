// AlashAnalog: аналоговые датчики (влажность почвы, освещённость, уровень воды, газ MQ, звук, потенциометр, напряжение батареи):
// проценты по калибровке, сглаживание, медиана, пороги с гистерезисом, автокалибровка, слова вместо чисел.
// Без зависимостей. MIT License. Alash Electronics, https://github.com/Alash-electronics/AlashAnalog
#ifndef ALASH_ANALOG_H
#define ALASH_ANALOG_H

#include <Arduino.h>

class AlashAnalog {
 public:
  // Вид датчика: от него зависят слова describe(). GENERIC без слов.
  enum Kind : uint8_t { GENERIC = 0, SOIL, LIGHT, WATER, GAS, SOUND };
  enum Error : uint8_t {
    ERR_OK = 0,
    ERR_STUCK_LOW,     // значение всегда около 0: нет питания датчика или провод на землю
    ERR_STUCK_HIGH,    // значение всегда около максимума: обрыв сигнала или провод на питание
    ERR_FLOATING,      // значение шумит само по себе: сигнальный провод никуда не подключён
    ERR_NO_RANGE       // диапазон не задан: проценты недоступны (вызовите setRange или autoRange)
  };
  enum Language : uint8_t { LANG_RU = 0, LANG_KK = 1, LANG_EN = 2 };

  AlashAnalog(uint8_t pin, Kind kind = GENERIC) : _pin(pin), _kind(kind) {}

  void begin();

  // ---- Значения -------------------------------------------------------------------
  int raw();                                // «сырое» значение АЦП
  int filtered();                           // сглаженное (медиана по нескольким измерениям)
  float percent();                          // 0..100 % по калибровке; NAN, если диапазон не задан
  float voltage();                          // напряжение на пине, В (с учётом делителя setDivider)
  const __FlashStringHelper *describe();    // словами по виду датчика: "сухо", "темно", "газ!"...

  // ---- Калибровка --------------------------------------------------------------------
  // low: сырое значение, соответствующее 0 %, high: соответствующее 100 %. Если low > high, шкала обращается (так у датчика влажности почвы).
  void setRange(int low, int high) { _low = low; _high = high; _hasRange = low != high; }
  void calibrateLow(uint8_t samples = 20)  { _low = averageRaw(samples);  _hasRange = _low != _high; }   // датчик в состоянии 0 % (например, сухой на воздухе)
  void calibrateHigh(uint8_t samples = 20) { _high = averageRaw(samples); _hasRange = _low != _high; }   // датчик в состоянии 100 % (например, в воде)
  bool autoRange(uint16_t durationMs = 10000);   // двигайте датчик между крайними состояниями: границы найдутся сами
  int lowValue() const { return _low; }
  int highValue() const { return _high; }
  void printCalibration(Print &out);        // строку setRange(...) вставить в скетч

  // ---- Настройки -----------------------------------------------------------------------
  void setSmoothing(uint8_t samples) { _samples = samples < 1 ? 1 : samples > 15 ? 15 : samples; }   // по скольким измерениям брать медиану (по умолчанию 5)
  void setAdcMax(int maxValue) { _adcMax = maxValue; _adcSet = true; }      // 1023 для Uno, 4095 для ESP32 (определяется само)
  void setReference(float volts) { _vref = volts; _refSet = true; }         // напряжение питания АЦП, В: 5.0 для Uno, 3.3 для ESP32
  void setDivider(float ratio) { _divider = ratio; }        // делитель напряжения: ratio = (R1 + R2) / R2, например 2.0 для двух равных

  // ---- Пороги с гистерезисом -------------------------------------------------------------
  void setThreshold(float lowPercent, float highPercent);   // true выше highPercent, false ниже lowPercent, в середине без изменений
  bool isHigh();                            // сейчас выше порога
  bool changed();                           // состояние порога только что изменилось (true один раз)

  // ---- Диагностика ---------------------------------------------------------------------
  Error error() const { return _err; }
  void setLanguage(Language lang) { _lang = lang; }
  const __FlashStringHelper *errorText();
  void diagnose(Print &out);                // читает пин 2 секунды и говорит, что не так

 private:
  uint8_t _pin;
  Kind _kind;
  int _low = 0, _high = 0;
  bool _hasRange = false, _started = false, _adcSet = false, _refSet = false;
  uint8_t _samples = 5;
  int _adcMax = 1023;
  float _vref = 5.0f;
  float _divider = 1.0f;
  float _thLow = 40, _thHigh = 60;
  bool _thOn = false, _thSet = false, _thChanged = false;
  Error _err = ERR_OK;
  Language _lang = LANG_RU;

  int averageRaw(uint8_t samples);
  void updateThreshold(float pct);
};

#endif  // ALASH_ANALOG_H
