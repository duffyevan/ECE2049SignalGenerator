#ifndef TIMER_H
#define TIMER_H
__interrupt void Timer_A2_ISR(void);
void setupTimerA2();
void timerA2InterruptEnable();
void timerA2InterruptDisable();
void setMaxCount(unsigned int count);
void set50HzTriangleWave();
void set100HzSawtoothWave();
#endif
