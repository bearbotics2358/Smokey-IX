/*
 * Prefs.h
 *
 *  Created on: Jan 14, 2016
 *      Author: hstechclub
 */

#ifndef SRC_PREFS_H_
#define SRC_PREFS_H_

#define LIGHT_RING_CONTROLLER_ADDRESS 0x4

#define JOYSTICK_PORT 0
#define JOYSTICKTWO_PORT 5

#define PDP_PORT 3

#define PCM_PORT 2

#define BACK_LEFT_ONE 13
#define BACK_LEFT_TWO 11
#define BACK_RIGHT_ONE 12
#define BACK_RIGHT_TWO 10

#define LEFT_ENCODER_PORT_A 0
#define LEFT_ENCODER_PORT_B 1
#define RIGHT_ENCODER_PORT_A 2
#define RIGHT_ENCODER_PORT_B 3

#define WINCH 14
#define WINCH_PORT_A 5
#define WINCH_PORT_B 6
#define WINCH_SWITCH_PORT 7

#define FINGER 17
#define FINGER_ENCODER_PORT 0

#define COLLECTOR 15
#define COLLECTOR_ENCODER_PORT 1
#define COLLECTOR_SWITCH_PORT 9

#define SHOOTER 16
#define SHOOTER_ENCODER_PORT 2

#define ROLLER 18
#define ROLLER_SWITCH_PORT 4


#define LEFT_SOL_PORT_ONE 0
#define LEFT_SOL_PORT_TWO 1

#define RIGHT_SOL_PORT_ONE 2
#define RIGHT_SOL_PORT_TWO 3

#define ROBOT_LENGTH 31.0 // This is in inches
#define ROBOT_PIVOT_POINT 6.5 //From back of frame in inches
#define TOWER_DISTANCE 109.5 //distance from face of tower when shooting- worked on saturday (3/12) and beginning of monday
// #define TOWER_DISTANCE 126.0

// tank conversion factor doesn't work, ending part to make it work
#define TANK_CONVERSION_FACTOR ((3.2 * 3.14159265) / 1000.0 * (145.0 / 85.0))

#define BAUD_RATE 9600
#define DATA_BITS 8

#endif /* SRC_PREFS_H_ */
