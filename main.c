

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


void main(void)
{
    //LaunchPad_Init();
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
}
