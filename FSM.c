/*
 * FSM.c
 *
 *  Created on: Mar 15, 2022
 *      Author: mpkai
 */

/*
 * FSM_Controller.c
 *
 *  Created on: Feb 17, 2022
 *      Author: Grant
 */

#include <stdint.h>
#include "msp.h"
#include "Motor.h"
#include "Reflectance.h"
#include "BumpInt.h"
#include "SysTickInts.h"
#include "Clock.h"

struct State {
  uint16_t DutyR;             
  uint16_t DutyL; 
  uint8_t dir;
  uint16_t time;
  uint8_t inc;
  const struct State *Next[4];};// depends on 2-bit input
typedef const struct State State_t;
#define center    &FSM[0]
#define left1     &FSM[1]
#define left2     &FSM[2]
#define right1    &FSM[3]
#define right2    &FSM[4]
#define lostLeft  &FSM[5]
#define lostRight &FSM[6]
#define lostMove1  &FSM[7]
#define lostMove2 &FSM[8]
#define recoveryTurnRight &FSM[9]
#define recoveryTurnLeft &FSM[10]
#define turn90 &FSM[11]
//#define turn90Boost &FSM[12]
#define stopShort &FSM[12]
#define stop      &FSM[13]

uint16_t count = 0;
State_t FSM[14]={
    {3000,3000,1,15,0,{right1,left1,right1,center}},
    {4750,6750,2,50,0,{lostLeft,left2,right1,center}},
    {1500,1500,1,10,0,{lostLeft,left1,right1,center}},
    {4750,6750,3,50,0,{lostRight,left1,right2,center}},
    {1500, 1500,1,10,0,{lostRight,left1,right1,center}},
    {5000, 7500,2,10,0,{lostMove1,lostMove1,lostMove1,lostMove1}},
    {7500, 5000,3,10,0,{lostMove2,lostMove2,lostMove2,lostMove2}},

    {3500, 2000,1,10,0,{recoveryTurnRight,left1,right1,center}},
    {2000, 3500,1,10,0,{recoveryTurnRight,left1,right1,center}},

    {0,2000,1,5,1,{recoveryTurnRight,left1,right1,center}},
    {2000,0,1,5,1,{recoveryTurnLeft,left1,right1,center}},

    {3000,3000,2,650,0,{center,center,center,center}},
    //{2000,0,1,5,0,{recoveryTurnLeft,left1,right1,center}},
    {0,0,1,10,0,{turn90,turn90,turn90,turn90}},

    {0, 0,1,100,0,{stop,stop,stop,stop}}};

State_t *Pt = center;  // state pointer

void motorOut(uint8_t dir,uint16_t dutyR,uint16_t dutyL){
  if(dir == 1){
    Motor_Forward(dutyL, dutyR);
  }
  else if(dir == 2){
    Motor_Right(dutyL, dutyR);
  }
  else if(dir == 3){
    Motor_Left(dutyL, dutyR);
  }

}

void advanceState(uint8_t Input){
  Pt = Pt->Next[Input];      // transition to next state
  motorOut(Pt->dir, Pt->DutyR, Pt->DutyL);   //move motor
  Clock_Delay1ms(Pt->time);

  if(Pt->inc == 0)
  { count = 0; }
  count += Pt->inc;

  if(count == 600 && Pt == recoveryTurnRight)
  {
      count = 0;
      Pt = recoveryTurnLeft;
  }
  else if(count == 900 && Pt == recoveryTurnLeft)
  {
      count = 0;
      Pt = stop;
  }

}

void stopMotor()
{
    Pt = stop;
}

void startTurn(){
  Pt = stopShort;
}

/*void turn90()
{
    Motor_Stop();
    Pt = stop;
    Motor_Right(3000,3000);
    Clock_Delay1ms(650);          // run for a while and stop

    Motor_Stop();
    Clock_Delay1ms(2000);
//    Motor_Forward(1000,1000);
//    Clock_Delay1ms(250);
//    Clock_Delay1ms(4000);

    Pt = center;
   Clock_Delay1ms(4000);


}*/
