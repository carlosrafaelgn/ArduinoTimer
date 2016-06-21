//******************************************************************
// Copyright (c) 2013, Carlos Rafael Gimenes das Neves
// This projected is licensed under the terms of the FreeBSD
// License. See LICENSE.txt for more details.
//
// https://github.com/carlosrafaelgn/ArduinoTimer
//
// ATmega168, ATmega328:
// - Using Timer 1 disables PWM (analogWrite) on pins 9 and 10
// ATmega2560:
// - Using Timer 1 disables PWM (analogWrite) on pins 11 and 12
// - Using Timer 3 disables PWM (analogWrite) on pins 2, 3 and 5
// - Using Timer 4 disables PWM (analogWrite) on pins 6, 7 and 8
// - Using Timer 5 disables PWM (analogWrite) on pins 44, 45 and 46
//******************************************************************

#include <Timer1.h>

unsigned long myMillis;

void setup()
{
  // Disable Arduino's default millisecond counter (from now on, millis(), micros(),
  // delay() and delayMicroseconds() will not work)
  disableMillis();
  
  // Prepare Timer1 to send notifications (interrupts) every 1000us (1ms)
  // On 16 MHz Arduino boards, this function has a resolution of 4us for intervals <= 260000,
  // and a resolution of 16us for other intervals
  // On 8 MHz Arduino boards, this function has a resolution of 8us for intervals <= 520000,
  // and a resolution of 32us for other intervals
  startTimer1(1000);
  
  // Initialize our counter
  myMillis = 0;
}

void loop()
{
}

// Define the function which will handle the notifications (interrupts)
ISR(timer1Event)
{
  // Reset Timer1 (resetTimer1 should be the first operation for better timer precision)
  resetTimer1();
  // For a smaller and faster code, the line above could safely be replaced with a call
  // to the function resetTimer1Unsafe() as, despite its name, it IS safe to call
  // that function in here (interrupts are disabled)
  
  // Make sure to do your work as fast as possible, since interrupts are automatically
  // disabled when this event happens (refer to interrupts() and noInterrupts() for
  // more information on that)
  
  myMillis++;
}
