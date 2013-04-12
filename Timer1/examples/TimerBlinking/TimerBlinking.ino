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
  // Toggle led's state
  ledState ^= 1;
  digitalWrite(LED, ledState);
}
