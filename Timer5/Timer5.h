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
#ifndef Timer5_h
#define Timer5_h

// Reference documentation: http://www.atmel.com/devices/atmega2560.aspx
//
// Timer 1 is available on ATmega168, ATmega328 and on ATmega2560
// All other timers are only available on ATmega2560
//
// (The page and chapter numbers refer to the ATmega2560 documentation)

//****************************************************************************
// ATmega2560: Using Timer 5 disables PWM (analogWrite) on pins 44, 45 and 46
//****************************************************************************

#include <inttypes.h>

// 16.9.6 TIMSK0 – Timer/Counter Interrupt Mask Register (page 134)
#ifndef disableMillis
  #define disableMillis() TIMSK0 &= ~1
#endif
#ifndef enableMillis
  #define enableMillis() TIMSK0 |= 1
#endif
#ifndef microsFromCounting
  #if (F_CPU == 16000000L)
    #define microsFromCounting(COUNTING) ((COUNTING) << 2)
  #elif (F_CPU == 8000000L)
    #define microsFromCounting(COUNTING) ((COUNTING) << 3)
  #else
    #error("Unsupported CPU frequency")
  #endif
#endif
#ifndef microsFromSlowCounting
  #if (F_CPU == 16000000L)
    #define microsFromSlowCounting(SLOWCOUNTING) ((SLOWCOUNTING) << 4)
  #elif (F_CPU == 8000000L)
    #define microsFromSlowCounting(SLOWCOUNTING) ((SLOWCOUNTING) << 5)
  #else
    #error("Unsupported CPU frequency")
  #endif
#endif
#ifndef microsFromUltraSlowCounting
  #if (F_CPU == 16000000L)
    #define microsFromUltraSlowCounting(SLOWCOUNTING) ((SLOWCOUNTING) << 6)
  #elif (F_CPU == 8000000L)
    #define microsFromUltraSlowCounting(SLOWCOUNTING) ((SLOWCOUNTING) << 7)
  #else
    #error("Unsupported CPU frequency")
  #endif
#endif

extern uint8_t __timer5Control;
extern uint16_t __timer5CounterValue;
#define readTimer5Unsafe() TCNT5
#define resetTimer5Unsafe() TCNT5 = __timer5CounterValue
#define pauseTimer5() TCCR5B = 0
#define resumeTimer5() TCCR5B = __timer5Control
extern void startTimer5(uint32_t microsecondsInterval);
extern void startCountingTimer5(void);
extern void startSlowCountingTimer5(void);
extern void startUltraSlowCountingTimer5(void);
extern uint16_t readTimer5(void);
extern void resetTimer5(void);

// 17.9.1 Normal Mode (page 149)
// The simplest mode of operation is the Normal mode (WGMn3:0 = 0). In this mode the counting
// direction is always up (incrementing), and no counter clear is performed. The counter simply
// overruns when it passes its maximum 16-bit value (MAX = 0xFFFF) and then restarts from the
// BOTTOM (0x0000). In normal operation the Timer/Counter Overflow Flag (TOVn) will be set in
// the same timer clock cycle as the TCNTn becomes zero. The TOVn Flag in this case behaves
// like a 17th bit, except that it is only set, not cleared. However, combined with the timer overflow
// interrupt that automatically clears the TOVn Flag, the timer resolution can be increased by software.
// There are no special cases to consider in the Normal mode, a new counter value can be
// written anytime.
#define timer5Event TIMER5_OVF_vect

#endif
