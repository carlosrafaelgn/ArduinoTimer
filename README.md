ArduinoTimer
============

ArduinoTimer is a library, which aims at providing developers with means to schedule asynchronous timer execution, pretty much like timer services provided by real OSes. In fact, it is a set of libraries, one for each timer available in ATmega2560 (1, 3, 4 and 5). By dividing the full library into four smaller libraries, the developer can save both RAM and Flash memory. The file ArduinoTimer.zip is a convenience for downloading all files at once :)

**You can download ArduinoTimer library in a single zip file [here](https://github.com/carlosrafaelgn/ArduinoTimer/blob/master/ArduinoTimer.zip?raw=true)!**

Library Timer1 can be used with ATmega2560, ATmega168 and with ATmega328. Nevertheless, libraries Timer3, Timer4 and Timer5 are exclusive to ATmega2560.

Each library has nine functions:
- void startTimer#(unsigned long microsecondsInterval): Starts the timer, and schedules the first notification. On 16 MHz Arduino boards, this function has a resolution of 4us, for intervals <= 260000, and a resolution of 16us for greater intervals. On 8 MHz Arduino boards, this function has a resolution of 8us, for intervals <= 520000, and a resolution of 32us for greater intervals. 
- void startCountingTimer#(): Starts the timer, but does not schedule any notifications. On 16 MHz Arduino boards, the timer has a resolution of 4us. On 8 MHz Arduino boards, the timer has a resolution of 8us. In other words, the value returned by readTimer#() should be multiplied either by 4 ou by 8 to get the actual amount of microseconds. The value returned by readTimer#() resets approximately every 262ms on 16 MHz boards, and every 524ms on 8 MHz boards.
- void startSlowCountingTimer#(): Starts the timer, but does not schedule any notifications. On 16 MHz Arduino boards, the timer has a resolution of 16us. On 8 MHz Arduino boards, the timer has a resolution of 32us. In other words, the value returned by readTimer#() should be multiplied either by 16 ou by 32 to get the actual amount of microseconds. The value returned by readTimer#() resets approximately every 1048ms on 16 MHz boards, and every 2097ms on 8 MHz boards.
- void resetTimer#(): Resets the timer's counter. This function should be called as soon as a notification is received, in order to properly prepare the timer for the next notification.
- void resetTimer#Unsafe(): A much faster version of the above function, but this one requires interrupts to be disabled.
- void pauseTimer#(): Pauses the timer, halting the counting and thus, preventing any further notifications.
- void resumeTimer#(): Resumes the timer.
- unsigned int readTimer#(): Returns the current value of the timer's counter.
- unsigned int readTimer#Unsafe(): A much faster version of the above function, but this one requires interrupts to be disabled.

Where # is 1, 3, 4 or 5, depending on the chosen library.

There are also four other functions common to all libraries:
- void disableMillis(): Disables Arduino's default millisecond counter, rendering millis(), micros(), delay() and delayMicroseconds() useless, while saving some processing power.
- void enableMillis(): Enables Arduino's default millisecond counter.
- void microsFromCounting(x): Returns the amount of microseconds in x (refer to Example 2 for sample usage).
- void microsFromSlowCounting(x): Returns the amount of microseconds in x - only to be used when the timer was started with startSlowCountingTimer#() (refer to Example 3 for sample usage).

Functions resetTimer#, pauseTimer#, resumeTimer#, disableMillis, enableMillis, microsFromCounting, microsFromSlowCounting are actually implemented as macros for better performance.

In order to receive the notifications, an interrupt handler must be setup as shown below:

``` c++
ISR(timer#Event)
{
  resetTimer#();
  // Handler code
}
```

Reference documentation:

http://www.atmel.com/devices/atmega2560.aspx

http://www.atmel.com/devices/atmega328.aspx

<hr/>

Timers and PWM (analogWrite)
----------------------------

When using the timers, a few PWM outputs (analogWrite) are disabled depending on the timer used.

ATmega168, ATmega328:
- Using Timer 1 disables PWM (analogWrite) on pins 9 and 10

ATmega2560:
- Using Timer 1 disables PWM (analogWrite) on pins 11 and 12
- Using Timer 3 disables PWM (analogWrite) on pins 2, 3 and 5
- Using Timer 4 disables PWM (analogWrite) on pins 6, 7 and 8
- Using Timer 5 disables PWM (analogWrite) on pins 44, 45 and 46

<hr/>


Example 1 - TimerBlinking
-------------------------
``` c++
#include <Timer1.h>

// Pin 13 has a LED connected on most Arduino boards
#define LED 13
byte ledState;

void setup()
{
  ledState = 0;
  // Disable Arduino's default millisecond counter (from now on, millis(), micros(),
  // delay() and delayMicroseconds() will not work)
  disableMillis();
  // Prepare Timer1 to send notifications every 1000000us (1s)
  // On 16 MHz Arduino boards, this function has a resolution of 4us for intervals <= 260000,
  // and a resolution of 16us for other intervals
  // On 8 MHz Arduino boards, this function has a resolution of 8us for intervals <= 520000,
  // and a resolution of 32us for other intervals
  startTimer1(1000000L);
  pinMode(LED, OUTPUT);
}

void loop()
{
}

// Define the function which will handle the notifications
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
  
  // Toggle led's state
  ledState ^= 1;
  digitalWrite(LED, ledState);
}
```

Example 2 - TimerCounting
-------------------------
``` c++
#include <Timer1.h>

unsigned int lastTime;

void setup()
{
  // Disable Arduino's default millisecond counter (from now on, millis(), micros(),
  // delay() and delayMicroseconds() will not work)
  disableMillis();
  // Prepare Timer1 to count
  // On 16 MHz Arduino boards, this function has a resolution of 4us
  // On 8 MHz Arduino boards, this function has a resolution of 8us
  startCountingTimer1();
  lastTime = readTimer1();
}

void loop()
{
  unsigned int now = readTimer1(), delta, deltamicros;
  delta = now - lastTime;
  // If you estimate deltamicros could be > 65 ms, or 65535 us,
  // delta should be cast to unsigned long, and deltamicros should be
  // created as an unsigned long variable
  deltamicros = microsFromCounting(delta);
  
  // Do your work here
  
  lastTime = now;
}
```

Example 3 - TimerSlowCounting
-------------------------
``` c++
#include <Timer1.h>

unsigned int lastTime;

void setup()
{
  // Disable Arduino's default millisecond counter (from now on, millis(), micros(),
  // delay() and delayMicroseconds() will not work)
  disableMillis();
  // Prepare Timer1 to count
  // On 16 MHz Arduino boards, this function has a resolution of 16us
  // On 8 MHz Arduino boards, this function has a resolution of 32us
  startSlowCountingTimer1();
  lastTime = readTimer1();
}

void loop()
{
  unsigned int now = readTimer1(), delta;
  unsigned long deltamicros;
  delta = now - lastTime;
  // If you estimate deltamicros will always be <= 65 ms, or 65535 us, you
  // can remove the type cast and create deltamicros as unsigned int
  deltamicros = microsFromSlowCounting((unsigned long)delta);
  
  // Do your work here
  
  lastTime = now;
}
```

Example 4 - TimerNotificationCounting
-------------------------------------
``` c++
#include <Timer1.h>

unsigned int myMillis;

void setup()
{
  myMillis = 0;
  // Disable Arduino's default millisecond counter (from now on, millis(), micros(),
  // delay() and delayMicroseconds() will not work)
  disableMillis();
  // Prepare Timer1 to send notifications every 1000us (1ms)
  // On 16 MHz Arduino boards, this function has a resolution of 4us for intervals <= 260000,
  // and a resolution of 16us for other intervals
  // On 8 MHz Arduino boards, this function has a resolution of 8us for intervals <= 520000,
  // and a resolution of 32us for other intervals
  startTimer1(1000);
}

void loop()
{
}

// Define the function which will handle the notifications
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
```

----

This projected is licensed under the terms of the FreeBSD License. See LICENSE.txt for more details.
