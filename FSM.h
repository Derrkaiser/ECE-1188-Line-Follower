/*
 * FSM.h
 *
 *  Created on: Mar 15, 2022
 *      Author: mpkai
 */

#ifndef FSM_H_
#define FSM_H_

/*
Determine which motor function to call and call it
using the states left and right duty parameters
*/
void motorOut(uint8_t dir,uint16_t dutyR,uint16_t dutyL);

/*
Output the current state and advance to the next one
based on provided input
*/
void advanceState(void);

#endif /* FSM_H_ */
