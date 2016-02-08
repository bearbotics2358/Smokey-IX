/*
 * SmokeyIX.h
 *
 *  Created on: Jan 14, 2016
 *      Author: hstechclub
 */

#include "ShifterController.h"
#include "Tank.h"
#include "CanTalonSRX.h"
#include "JrimmyGyro.h"
#include "I2C.h"
#include "PivotArm.h"
#include "Roller.h"
#include "Lifter.h"
#include "Shooter.h"

#ifndef SRC_SMOKEYIX_H_
#define SRC_SMOKEYIX_H_

enum AutoState {
	kMoveToLowBar = 0,
	kMoveUnderLowBar,
	kMovePerp,
	kTurnPerp,
	kCheckAim,
	kAdjust,
	kShoot,
	kTurnBack,
	kDriveToC,
	kAutoIdle
};

class SmokeyIX : public IterativeRobot {
public:
	SmokeyIX(void);
	void RobotInit(void);
	void TeleopInit(void);
	void TeleopPeriodic(void);
	void TestInit(void);
	void TestPeriodic(void);
	void AutonomousInit(void);
	void AutonomousPeriodicFull(void);
	void AutonomousPeriodic(void);
	void AutonomousPeriodicSimple(void);

private:
	Joystick a_Joystick;
	Joystick a_Joystick2;

	PowerDistributionPanel a_PDP;

	Compressor a_Compressor;

	CanTalonSRX a_BLOne;
	CanTalonSRX a_BLTwo;
	CanTalonSRX a_BROne;
	CanTalonSRX a_BRTwo;

	Encoder a_LeftEncoder;
	Encoder a_RightEncoder;

	Lifter a_Winch;

	PivotArm a_Finger;

	PivotArm a_Collector;

	Shooter a_Shooter;

	Roller a_Roller;

	DoubleSolenoid a_LeftSol;

	JrimmyGyro a_Gyro;

	ShifterController a_Left;
	ShifterController a_Right;
	Tank a_Tank;

	AutoState a_AutoState;

};

#endif /* SRC_SMOKEYIX_H_ */
