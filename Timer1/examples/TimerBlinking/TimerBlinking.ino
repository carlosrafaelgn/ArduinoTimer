#include <Timer1.h>

// Pin 13 has an LED connected on most Arduino boards
#define LED 13
byte ledState;

void setup()
{
  ledState = 0;
  // Prepare Timer1 to send notifications every 1000000us (1s)
  startTimer1(1000000);
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
  // to the function resetTimer1Unsafe() as, despite of its name, it IS safe to call
  // that function in here (interrupts are disabled)
  
  // Make sure to do your work as fast as possible, since interrupts are automatically
  // disabled when this event happens (refer to interrupts() and noInterrupts() for
  // more information on that)
  
  // Toggle led's state
  ledState ^= 1;
  digitalWrite(LED, ledState);
}
