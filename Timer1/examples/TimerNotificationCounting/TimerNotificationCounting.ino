#include <Timer1.h>

unsigned int myMillis;

void setup()
{
  myMillis = 0;
  // Disable Arduino's default millisecond counter (from now on, millis(), micros(),
  // delay() and delayMicroseconds() will not work)
  disableMillis();
  // Prepare Timer1 to send notifications every 1000us (1ms)
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
  // to the function resetTimer1Unsafe() as, despite of its name, it IS safe to call
  // that function in here (interrupts are disabled)
  
  // Make sure to do your work as fast as possible, since interrupts are automatically
  // disabled when this event happens (refer to interrupts() and noInterrupts() for
  // more information on that)
  
  myMillis++;
}
