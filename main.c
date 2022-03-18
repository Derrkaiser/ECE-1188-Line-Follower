

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

#include "Bump.h" // Just used to show that motors work before BumpInt is completed. Once BumpInt is completed. Right click BumpInt.c in project explorer to the left and uncheck "Exclude from build". Then delete
                 // Bump.c and Bump.h


void checkBump()
{
    uint8_t bump_read = 0;

    bump_read = Bump_Read();
    /* Map LED's from Bump Sensor */

   if(!(bump_read & (1 << (7))))
   {
       Motor_Stop();
   }
   else if(!(bump_read & (1 << (6))))
   {
       Motor_Stop();
   }
   else if(!(bump_read & (1 << (5))))
   {
       Motor_Stop();
   }
   else if(!(bump_read & (1 << (3))))
   {
       Motor_Stop();
   }
   else if(!(bump_read & (1 << (2))))
   {
       Motor_Stop();
   }
   else if(!(bump_read & (1 << (0))))
   {
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
    Bump_Init();      // bump switches
    Motor_Init();     // your function
    TimerA1_Init(&checkBump,50000);  // 1000 Hz
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
