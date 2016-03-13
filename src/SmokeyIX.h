/*
 * SmokeyIX.h
 *
 *  Created on: Jan 14, 2016
 *      Author: hstechclub
 */

#include "ShifterController.h"
#include "Tank.h"
#include "JrimmyGyro.h"
#include "PivotArm.h"
#include "Roller.h"
#include "Lifter.h"
#include "Shooter.h"
#include "Vision/TargetDetector.h"

#ifndef SRC_SMOKEYIX_H_
#define SRC_SMOKEYIX_H_

enum AutoState {
	kDropCollector = 0,
	kMoveToLowBar,
	kMoveUnderLowBar,
	kMoveToShoot,
	kTurnToShoot,
	kTurnToShootWait,
	kMoveTowardsTower,
	kMoveTowardsTowerWait,
	// kLoadingBotWait,
	kLoadingBot,
	kLoaderDownWait,
	kLoaderDown,
	kCheckAimWait,
	kCheckAim,
	kAdjust,
	kShootWait,
	kShoot,
	kTurnWait,
	kTurnBack,
	kDriveToTurnPoint,
	kTurnToC,
	kDriveToC,
	kAutoIdle
};

class SmokeyIX : public IterativeRobot {
public:
	SmokeyIX(void);
	void RobotInit(void);
	void DisabledInit(void);
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

	Lifter a_Winch;

	PivotArm a_Finger;

	PivotArm a_Collector;

	DigitalInput a_CollectorSwitch;

	Shooter a_Shooter;

	Roller a_Roller;

	DoubleSolenoid a_LeftSol;

	JrimmyGyro a_Gyro;

	ShifterController a_Left;
	ShifterController a_Right;
	Tank a_Tank;

	AutoState a_AutoState;

	double tState;
	int shooterCurrent;
	int shooterStart;

	TargetDetector a_TargetDetector;
};

#endif /* SRC_SMOKEYIX_H_ */
