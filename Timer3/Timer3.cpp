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
#include "Timer3.h"

uint8_t __timer3Control;
uint16_t __timer3CounterValue;
// 0 < microsecondsInterval <= 1000000
// On 16 MHz Arduino boards, this function has a resolution of 4us, for intervals <= 260000, and a resolution of 16us for other intervals.
// On 8 MHz Arduino boards, this function has a resolution of 8us, for intervals <= 520000, and a resolution of 32us for other intervals.
void startTimer3(uint32_t microsecondsInterval) {
  pauseTimer3();
  // 18. Timer/Counter 0, 1, 3, 4, and 5 Prescaler (page 169)
  // 17.9.1 Normal Mode (page 149)
  TCCR3A = 0;
  TCCR3C = 0;
  // 17.11.5 TCCR3B (page 160)
  // 0 0 0 No clock source (Timer/Counter stopped)
  // 0 0 1 clkIO/1 (No prescaling)
  // 0 1 0 clkIO/8 (From prescaler)
  // 0 1 1 clkIO/64 (From prescaler)
  // 1 0 0 clkIO/256 (From prescaler)
  // 1 0 1 clkIO/1024 (From prescaler)
#if (F_CPU == 16000000L)
  if (microsecondsInterval <= 260000) {
    __timer3Control = B00000011;
    // The proper way of doing this would be:
    // 65536 - (microsecondsInterval / 4)
    // But, in order to save one 32-bit operation, this "- 1" is necessary...
    __timer3CounterValue = 65535 - ((uint16_t)(microsecondsInterval >> 2) - 1);
  } else {
    __timer3Control = B00000100;
    __timer3CounterValue = 65535 - ((uint16_t)(microsecondsInterval >> 4) - 1);
  }
#elif (F_CPU == 8000000L)
  if (microsecondsInterval <= 520000) {
    __timer3Control = B00000011;
    __timer3CounterValue = 65535 - ((uint16_t)(microsecondsInterval >> 3) - 1);
  } else {
    __timer3Control = B00000100;
    __timer3CounterValue = 65535 - ((uint16_t)(microsecondsInterval >> 5) - 1);
  }
#else
  #error("Unsupported CPU frequency")
#endif
  resetTimer3();
  // 17.11.37 TIFR3 – Timer/Counter3 Interrupt Flag Register (page 167)
  TIFR3 = 0;
  TIMSK3 = 1;
  resumeTimer3();
}
void startCountingTimer3(void) {
  pauseTimer3();
  TCCR3A = 0;
  TCCR3C = 0;
#if (F_CPU == 16000000L) || (F_CPU == 8000000L)
  __timer3Control = B00000011;
  __timer3CounterValue = 0;
#else
  #error("Unsupported CPU frequency")
#endif
  resetTimer3();
  TIFR3 = 0;
  TIMSK3 = 0;
  resumeTimer3();
}
uint16_t readTimer3(void) {
  // 17.3 Accessing 16-bit Registers (page 138)
  uint8_t sreg;
  uint16_t i;
  // Save global interrupt flag
  // 7.4.1 SREG – AVR Status Register (page 14)
  sreg = SREG;
  // Disable interrupts
  cli();
  // Read TCNTn
  i = readTimer3Unsafe();
  // Restore global interrupt flag
  SREG = sreg;
  return i;
}
void resetTimer3(void) {
  // 17.3 Accessing 16-bit Registers (page 138)
  uint8_t sreg;
  // Save global interrupt flag
  // 7.4.1 SREG – AVR Status Register (page 14)
  sreg = SREG;
  // Disable interrupts
  cli();
  // Write TCNTn
  resetTimer3Unsafe();
  // Restore global interrupt flag
  SREG = sreg;
}
