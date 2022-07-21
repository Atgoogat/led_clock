#include <Arduino.h>
#include <RTClib.h>

#include "hang.h"
#include "led_clock.h"

#define SR_N_OE_PIN 7 // Output enabled (active low)
#define SR_N_MR_PIN 6 // Master Reset (active low)
#define SR_SHCP_PIN 5 // Shift Register Clock
#define SR_STCP_PIN 4 // Storage Register Clock 
#define SR_DS_PIN 3 // Data input

#define BTN_H_PIN 8
#define BTN_M_PIN 9

RTC_DS3231 rtc;

void setup() {
  pinMode(SR_N_OE_PIN, OUTPUT);
  digitalWrite(SR_N_OE_PIN, HIGH); // Disable all registers

  pinMode(SR_N_MR_PIN, OUTPUT);
  pinMode(SR_SHCP_PIN, OUTPUT);
  pinMode(SR_STCP_PIN, OUTPUT);
  pinMode(SR_DS_PIN, OUTPUT);

  pinMode(BTN_H_PIN, INPUT_PULLUP);
  pinMode(BTN_M_PIN, INPUT_PULLUP);

  // Reset all registers and active output
  digitalWrite(SR_N_MR_PIN, LOW);
  digitalWrite(SR_STCP_PIN, LOW);
  digitalWrite(SR_STCP_PIN, HIGH);
  digitalWrite(SR_N_MR_PIN, HIGH);
  digitalWrite(SR_N_OE_PIN, LOW);

  if (!rtc.begin()) {
    hang();
  }
  if (rtc.lostPower()) {
    const DateTime zero;
    rtc.adjust(zero);
  }
}

uint32_t last_mask = -1;
int last_btn_h_state = HIGH;
int last_btn_m_state = HIGH;
void loop() {

  const auto now = rtc.now();
  const SimpleTime time{
    now.minute(),
    now.hour()
  };

  const auto mask = cvtSimpleTime2Mask(time);
  
  if (mask != last_mask) {
    digitalWrite(SR_STCP_PIN, LOW);
    for(int i=0; i<32; ++i) {
      const auto output = mask & (uint32_t{1} << (31-i));
      const auto digitalOutput = output == 0 ? LOW : HIGH;

      digitalWrite(SR_SHCP_PIN, LOW);
      digitalWrite(SR_DS_PIN, digitalOutput);
      digitalWrite(SR_SHCP_PIN, HIGH);
    }
    digitalWrite(SR_STCP_PIN, HIGH);
    last_mask = mask;
  }

  const int btn_h_state = digitalRead(BTN_H_PIN);
  if (btn_h_state == LOW && last_btn_h_state == HIGH) {
    const uint8_t hour = (int16_t{now.hour()} + 1) % 24;
    const DateTime next{2000, 1, 1, hour, now.minute(), 0};
    rtc.adjust(next);
  }
  last_btn_h_state = btn_h_state;

  const int btn_m_state = digitalRead(BTN_M_PIN);
  if (btn_m_state == LOW && last_btn_m_state == HIGH) {
    const uint8_t min = (int16_t{now.minute()} + 1) % 60;
    const DateTime next{2000, 1, 1, now.hour(), min, 0};
    rtc.adjust(next);
  }
  last_btn_m_state = btn_m_state;

  delay(10);
}