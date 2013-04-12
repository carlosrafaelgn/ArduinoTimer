ArduinoTimer
============

ArduinoTimer is a library, which aims at providing developers with means to schedule asynchronous timer execution, pretty much like timer services provided by real OSes. In fact, it is a set of libraries, one for each timer available in ATmega2560 (1, 3, 4 and 5). By dividing the full library into four smaller libraries, the developer can save both RAM and Flash memory.

Each library has six functions:
- void startTimerNNN(unsigned long microsecondsInterval): Starts the timer, and schedules the first notification. On 16 MHz Arduino boards, this function has a resolution of 4us, for intervals <= 260000, and a resolution of 16us for greater intervals. On 8 MHz Arduino boards, this function has a resolution of 8us, for intervals <= 520000, and a resolution of 32us for greater intervals. 
- void startCountingTimerNNN(): Starts the timer, but does not schedule any notifications. On 16 MHz Arduino boards, the timer has a resolution of 4us. On 8 MHz Arduino boards, the timer has a resolution of 8us. In other words, the value returned by readTimerNNN() should be multiplied either by 4 ou by 8 to get the actual amount of microseconds. The value returned by readTimerNNN() resets approximately every 262ms on 16 MHz boards, and every 524ms on 8 MHz boards.
- void resetTimerNNN(): Resets the timer's counter. This function should be called as soon as a notification is received, in order to properly prepare the timer for the next notification.
- void pauseTimerNNN(): Pauses the timer, halting the counting and thus, preventing any further notifications.
- void resumeTimerNNN(): Resumes the timer.
- unsigned int readTimerNNN(): Returns the current value of the timer's counter.

Where NNN is 1, 3, 4 or 5, depending on the chosen library.

There are also two other functions common to all libraries:
- void disableMillis(): Disables Arduino's default millisecond counter, rendering millis(), micros(), delay() and delayMicroseconds() useless, while saving some processing power.
- void enableMillis(): Enables Arduino's default millisecond counter.

Functions resetTimerNNN, pauseTimerNNN, resumeTimerNNN, disableMillis and enableMillis are actually implemented as macros for best performance.

In order to receive the notifications, an interrupt handler must be setup as shown below:

ISR(timerNNNEvent)<br/>{<br/>  resetTimerNNN();<br/>  // Handler code<br/>}


Reference documentation: http://www.atmel.com/devices/atmega2560.aspx
