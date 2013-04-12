#include <Timer1.h>

unsigned int lastTime;

void setup()
{
  // Disable Arduino's default millisecond counter (from now on, millis(), micros(),
  // delay() and delayMicroseconds() will not work)
  disableMillis();
  // Prepare Timer1 to count
  // (4us resolution on 16 MHz boards and 8us resolution on 8 MHz boards)
  startCountingTimer1();
  lastTime = readTimer1();
}

void loop()
{
  unsigned int now = readTimer1(), delta, deltamicros;
  delta = now - lastTime;
  // Multiply delta either by 4 or by 8, depending on the CPU frequency,
  // to obtain the amount of microseconds elapsed since last time
  // If you estimate this value could be > 65 ms, or 65535 us,
  // delta should be cast to unsigned long, and deltamicros should be
  // created as an unsigned long variable
  deltamicros = delta << 2; // Multiplying by 4 (<< 3 multiplies by 8)
  
  // Do your work here
  
  lastTime = now;
}
