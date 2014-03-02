#include <Timer1.h>

//******************************************************************
// ATmega168, ATmega328:
// - Using Timer 1 disables PWM (analogWrite) on pins 9 and 10
// ATmega2560:
// - Using Timer 1 disables PWM (analogWrite) on pins 11 and 12
// - Using Timer 3 disables PWM (analogWrite) on pins 2, 3 and 5
// - Using Timer 4 disables PWM (analogWrite) on pins 6, 7 and 8
// - Using Timer 5 disables PWM (analogWrite) on pins 44, 45 and 46
//******************************************************************

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
  // If you estimate this value could be > 65 ms, or 65535 us,
  // delta should be cast to unsigned long, and deltamicros should be
  // created as an unsigned long variable
  deltamicros = microsFromCounting(delta);
  
  // Do your work here
  
  lastTime = now;
}
