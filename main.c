

/**
 * main.c
 */
#include "msp.h"
#include "BumpInt.h"
#include "Clock.h"
#include "CortexM.h"
#include "FSM.h"
#include "LaunchPad.h"
#include "Motor.h"
#include "PWM.h"
#include "Reflectance.h"
#include "SysTickInts.h"
#include "TimerA1.h"

void HandleCollision(){

    uint8_t bumpResult = BumpInt_Read();

    if (!(bumpResult & (1 << 5))|!(bumpResult & (1 << 4))|!(bumpResult & (1 << 3))) {
       Motor_Stop();
    }
    if (!(bumpResult & (1 << 2))|!(bumpResult & (1 << 1))|!(bumpResult & (1 << 0))) {
        Motor_Stop();
    }
}

void TimedPause(uint32_t time){
  Clock_Delay1ms(time);          // run for a while and stop
  Motor_Stop();
  while(LaunchPad_Input()==0);  // wait for touch
  while(LaunchPad_Input());     // wait for release
}

void main(void)
{
    Clock_Init48MHz();
    LaunchPad_Init(); // built-in switches and LEDs
    BumpInt_Init(&HandleCollision);      // bump switches
    Motor_Init();     // your function
    TimerA1_Init(&HandleCollision,50000);  // 1000 Hz
    EnableInterrupts();

    while(1){
      TimedPause(4000); // Press a button to continue
      Motor_Forward(7500,7500);  // your function
      TimedPause(2000);
      Motor_Backward(7500,7500); // your function
      TimedPause(3000);
      Motor_Left(5000,5000);     // your function
      TimedPause(3000);
      Motor_Right(5000,5000);    // your function
    }
}
