

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
       stopMotor();
    }
    if (!(bumpResult & (1 << 2))|!(bumpResult & (1 << 1))|!(bumpResult & (1 << 0))) {
        Motor_Stop();
        stopMotor();
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
    Reflectance_Init();
    TimerA1_Init(&HandleCollision,50000);  // 1000 Hz
    EnableInterrupts();

    while(1){
      uint8_t data = Reflectance_Read(1000);

      if(data & 0b00111111 == 0b00111111)
      {
          startTurn();
          continue;
      }
      uint8_t center_data = data & 0b00011000;
      center_data = center_data >> 3;
      advanceState(center_data);
    }
}
