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
  const struct State *Next[4];};// depends on 2-bit input
typedef const struct State State_t;
#define center    &FSM[0]
#define left1     &FSM[1]
#define left2     &FSM[2]
#define right1    &FSM[3]
#define right2    &FSM[4]
#define lostLeft  &FSM[5]
#define lostRight &FSM[6]
#define lostMove  &FSM[7]
#define stop      &FSM[8]

State_t FSM[9]={
 {7500,7500,1,{right1,left1,right1,center}},
 {2500,7500,2,{lostLeft,left2,right1,center}},
 {7500,7500,1,{lostLeft,left1,right1,center}},
 {7500,2500,3,{lostRight,left1,right2,center}},
 {7500, 7500,1,{lostRight,left1,right1,center}},
 {0, 7500,2,{lostMove,lostMove,lostMove,lostMove}},
 {7500, 0,3,{lostMove,lostMove,lostMove,lostMove}},
 {7500, 7500,1,{stop,left1,right1,center}},
 {0, 0,1,{stop,stop,stop,stop}}};

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
  motorOut(Pt->dir, Pt->DutyL, Pt->DutyR);   //move motor
}
