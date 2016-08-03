//
// ArduinoTimer is distributed under the FreeBSD License
//
// Copyright (c) 2013, Carlos Rafael Gimenes das Neves
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice, this
//    list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
// ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// The views and conclusions contained in the software and documentation are those
// of the authors and should not be interpreted as representing official policies,
// either expressed or implied, of the FreeBSD Project.
//
// https://github.com/carlosrafaelgn/ArduinoTimer
//
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include "Timer1.h"

uint8_t __timer1Control;
uint16_t __timer1CounterValue;
// On 16 MHz Arduino boards, this function has a resolution of 4us, for intervals <= 262000, a resolution of 16us for intervals <= 1048000, and a resolution of 64us for intervals <= 4194000
// On 8 MHz Arduino boards, this function has a resolution of 8us, for intervals <= 524000, a resolution of 32us for intervals <= 2097000, and a resolution os 128us for intervals <= 8388000
void startTimer1(uint32_t microsecondsInterval) {
  pauseTimer1();
  // 18. Timer/Counter 0, 1, 3, 4, and 5 Prescaler (page 169)
  // 17.9.1 Normal Mode (page 149)
  TCCR1A = 0;
  TCCR1C = 0;
  // 17.11.5 TCCR1B (page 160)
  // 0 0 0 No clock source (Timer/Counter stopped)
  // 0 0 1 clkIO/1 (No prescaling)
  // 0 1 0 clkIO/8 (From prescaler)
  // 0 1 1 clkIO/64 (From prescaler)
  // 1 0 0 clkIO/256 (From prescaler)
  // 1 0 1 clkIO/1024 (From prescaler)
#if (F_CPU == 16000000L)
  if (microsecondsInterval <= 262000L) {
    __timer1Control = B00000011;
    // The proper way of doing this would be:
    // 65536 - (microsecondsInterval / 4)
    // But, in order to save one 32-bit operation, this "- 1" is necessary...
    __timer1CounterValue = 65535 - ((uint16_t)(microsecondsInterval >> 2) - 1);
  } else if (microsecondsInterval <= 1048000L) {
    __timer1Control = B00000100;
    __timer1CounterValue = 65535 - ((uint16_t)(microsecondsInterval >> 4) - 1);
  } else {
    __timer1Control = B00000101;
    __timer1CounterValue = 65535 - ((uint16_t)(microsecondsInterval >> 6) - 1);
  }
#elif (F_CPU == 8000000L)
  if (microsecondsInterval <= 524000L) {
    __timer1Control = B00000011;
    __timer1CounterValue = 65535 - ((uint16_t)(microsecondsInterval >> 3) - 1);
  } else if (microsecondsInterval <= 2097000L) {
    __timer1Control = B00000100;
    __timer1CounterValue = 65535 - ((uint16_t)(microsecondsInterval >> 5) - 1);
  } else {
    __timer1Control = B00000101;
    __timer1CounterValue = 65535 - ((uint16_t)(microsecondsInterval >> 7) - 1);
  }
#else
  #error("Unsupported CPU frequency")
#endif
  resetTimer1();
  // 17.11.37 TIFR1 – Timer/Counter1 Interrupt Flag Register (page 167)
  TIFR1 = 0;
  TIMSK1 = 1;
  resumeTimer1();
}
// On 16 MHz Arduino boards, this function has a resolution of 4us
// On 8 MHz Arduino boards, this function has a resolution of 8us
void startCountingTimer1(void) {
  pauseTimer1();
  TCCR1A = 0;
  TCCR1C = 0;
#if (F_CPU == 16000000L) || (F_CPU == 8000000L)
  __timer1Control = B00000011;
  __timer1CounterValue = 0;
#else
  #error("Unsupported CPU frequency")
#endif
  resetTimer1();
  TIFR1 = 0;
  TIMSK1 = 0;
  resumeTimer1();
}
// On 16 MHz Arduino boards, this function has a resolution of 16us
// On 8 MHz Arduino boards, this function has a resolution of 32us
void startSlowCountingTimer1(void) {
  pauseTimer1();
  TCCR1A = 0;
  TCCR1C = 0;
#if (F_CPU == 16000000L) || (F_CPU == 8000000L)
  __timer1Control = B00000100;
  __timer1CounterValue = 0;
#else
  #error("Unsupported CPU frequency")
#endif
  resetTimer1();
  TIFR1 = 0;
  TIMSK1 = 0;
  resumeTimer1();
}
// On 16 MHz Arduino boards, this function has a resolution of 64us
// On 8 MHz Arduino boards, this function has a resolution of 128us
void startUltraSlowCountingTimer1(void) {
  pauseTimer1();
  TCCR1A = 0;
  TCCR1C = 0;
#if (F_CPU == 16000000L) || (F_CPU == 8000000L)
  __timer1Control = B00000101;
  __timer1CounterValue = 0;
#else
  #error("Unsupported CPU frequency")
#endif
  resetTimer1();
  TIFR1 = 0;
  TIMSK1 = 0;
  resumeTimer1();
}
uint16_t readTimer1(void) {
  // 17.3 Accessing 16-bit Registers (page 138)
  uint8_t sreg;
  uint16_t i;
  // Save global interrupt flag
  // 7.4.1 SREG – AVR Status Register (page 14)
  sreg = SREG;
  // Disable interrupts
  cli();
  // Read TCNTn
  i = readTimer1Unsafe();
  // Restore global interrupt flag
  SREG = sreg;
  return i;
}
void resetTimer1(void) {
  // 17.3 Accessing 16-bit Registers (page 138)
  uint8_t sreg;
  // Save global interrupt flag
  // 7.4.1 SREG – AVR Status Register (page 14)
  sreg = SREG;
  // Disable interrupts
  cli();
  // Write TCNTn
  resetTimer1Unsafe();
  // Restore global interrupt flag
  SREG = sreg;
}
