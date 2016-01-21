/*
 * SmokeyIX.h
 *
 *  Created on: Jan 14, 2016
 *      Author: hstechclub
 */

#include "ShifterController.h"
#include "Tank.h"
#include "CanTalonSRX.h"

#ifndef SRC_SMOKEYIX_H_
#define SRC_SMOKEYIX_H_

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

	Victor a_WinchOne;
	Victor a_WinchTwo;
	Victor a_CollectorOne;
	Victor a_CollectorTwo;
	Victor a_Roller;

	DoubleSolenoid a_LeftSol;


	ShifterController a_Left;
	ShifterController a_Right;
	Tank a_Tank;

};

#endif /* SRC_SMOKEYIX_H_ */
