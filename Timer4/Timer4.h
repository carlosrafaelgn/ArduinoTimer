#ifndef Timer4_h
#define Timer4_h

#include <inttypes.h>

// 16.9.2 TCCR0B – Timer/Counter Control Register B (page 132)
// TCCR0B has Original value is 3
// 16.9.6 TIMSK0 – Timer/Counter Interrupt Mask Register (page 134)
#ifndef disableMillis
#define disableMillis() TIMSK0 &= (~TIMSK0)
#endif
#ifndef enableMillis
#define enableMillis() TIMSK0 |= TIMSK0
#endif

extern uint8_t __timer4Control;
extern uint16_t __timer4CounterValue;
#define resetTimer4() TCNT4 = __timer4CounterValue
#define pauseTimer4() TCCR4B = 0
#define resumeTimer4() TCCR4B = __timer4Control
extern void startTimer4(uint32_t microsecondsInterval);
extern uint16_t readTimer4(void);

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
#define timer4Event TIMER4_OVF_vect

#endif
