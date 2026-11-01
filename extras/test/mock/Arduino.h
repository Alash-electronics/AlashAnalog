// Имитация Arduino.h для тестов аналоговых датчиков: analogRead берёт значения из функции, время идёт вручную.
#pragma once
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
class __FlashStringHelper;
#define F(x) (reinterpret_cast<const __FlashStringHelper *>(x))
inline uint32_t g_ms = 1000;
inline uint32_t millis() { return g_ms; }
inline void delay(uint32_t ms) { g_ms += ms; }
inline int (*g_adc)() = nullptr;
inline int analogRead(uint8_t) { return g_adc ? g_adc() : 0; }
class Print {
 public:
  void print(const char *s) { printf("%s", s); }
  void print(const __FlashStringHelper *s) { printf("%s", (const char *)s); }
  void print(int v) { printf("%d", v); }
  void println(const char *s = "") { printf("%s\n", s); }
  void println(int v) { printf("%d\n", v); }
  void println(const __FlashStringHelper *s) { printf("%s\n", (const char *)s); }
};
