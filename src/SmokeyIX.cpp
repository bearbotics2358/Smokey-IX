#include "SmokeyIX.h"
#include "Prefs.h"

#include <WPILib.h>
#include <math.h>
#include <stdio.h>

SmokeyIX::SmokeyIX(void):
		a_Joystick(JOYSTICK_PORT),
		a_Joystick2(JOYSTICKTWO_PORT),
		a_PDP(PDP_PORT),
		a_Compressor(PCM_PORT),
		a_Winch(WINCH, WINCH_PORT_A, WINCH_PORT_B, WINCH_SWITCH_PORT),
		a_Finger(FINGER, FINGER_ENCODER_PORT, 0, 0), // Third argument is our upper limit on the encoder, fourth is our lower limit
		a_Collector(COLLECTOR, COLLECTOR_ENCODER_PORT, 0, 0), // See above
		a_CollectorSwitch(COLLECTOR_SWITCH_PORT),
		a_Shooter(SHOOTER, SHOOTER_ENCODER_PORT),
		a_Roller(ROLLER, ROLLER_SWITCH_PORT),
		a_LeftSol(PCM_PORT, LEFT_SOL_PORT_ONE,LEFT_SOL_PORT_TWO),   // Must specify port # if not 0
		a_Gyro(I2C::kMXP), // Didn't work because we used smartdashboard in the constructor- wait to use it until after RobotInit()
		a_Left(BACK_LEFT_ONE, BACK_LEFT_TWO, a_LeftSol, LEFT_ENCODER_PORT_A, LEFT_ENCODER_PORT_B),
		a_Right(BACK_RIGHT_ONE, BACK_RIGHT_TWO, a_LeftSol, RIGHT_ENCODER_PORT_A, RIGHT_ENCODER_PORT_B),
		a_Tank(a_Left,a_Right),
		a_AutoState(kAutoIdle)
		// a_TargetDetector("10.23.58.11")
{

}

void SmokeyIX::RobotInit()
{
	// a_Compressor.SetClosedLoopControl(true);
	a_LeftSol.Set(DoubleSolenoid::kForward);
	a_Tank.Init();
}

void SmokeyIX::DisabledInit()
{
	a_Tank.Disable();
	// a_TargetDetector.StopProcessing();
}

void SmokeyIX::AutonomousInit()
{
	a_Gyro.Cal();
	a_AutoState = kMoveUnderLowBar;
	a_Tank.Enable();
}

void SmokeyIX::AutonomousPeriodic()
{
	AutoState nextState = a_AutoState;

	const double CONVERSION_FACTOR = 3.5/1000;
	int tankDistance = 0; //  encoder value * CONVERSION_FACTOR

	const double LOW_BAR_DISTANCE = 0.0; // 77.0" - ROBOT_LENGTH
	const double LOW_BAR_CLEAR = 0.0; // 48.0" + ROBOT_LENGTH
	const double TURN_SPOT_DISTANCE = 0.0; // 113.06" - ROBOT_PIVOT_POINT
	const double SHOOT_SPOT_DISTANCE = 0.0; // 130.9 - TOWER_DISTANCE
	const double TURN_ANGLE = 60.0;
	const double LOAD_HIGH_ANGLE = 0.0; // max angle of loader arm
	const double TURN_AROUND_ANGLE = (180 * M_1_PI) * asin(48.0/(sqrt(pow(SHOOT_SPOT_DISTANCE,2) - 96*sqrt(3)*SHOOT_SPOT_DISTANCE + 9216)));
	const double C_DISTANCE = (sqrt(pow(SHOOT_SPOT_DISTANCE,2) - 96*sqrt(3)*SHOOT_SPOT_DISTANCE + 9216));
	const double TURN_TO_C_ANGLE = 180.0; // check all these angles when testing
	const double TO_C_DISTANCE = TURN_SPOT_DISTANCE + ROBOT_PIVOT_POINT;

	switch (a_AutoState) {
	case kMoveToLowBar:
		if (tankDistance < LOW_BAR_DISTANCE) {
			a_Tank.AutonUpdate(0.5, 0.5);
		} else {
			a_Tank.AutonUpdate(0, 0);
			nextState = kMoveUnderLowBar;
		}
		break;
	case kMoveUnderLowBar:
		if (tankDistance < LOW_BAR_CLEAR) {
			a_Tank.AutonUpdate(0.5, 0.5); //change to a usable speed
		} else {
			a_Tank.AutonUpdate(0, 0);
			nextState = kMoveToShoot;
		}
		break;
	case kMoveToShoot:
		if (tankDistance < TURN_SPOT_DISTANCE) {
			a_Tank.AutonUpdate(0.5, 0.5);
		} else {
			a_Tank.AutonUpdate(0, 0);
			nextState = kTurnToShoot;
		}
		break;
	case kTurnToShoot:
		if (a_Gyro.GetAngle() < TURN_ANGLE) {
			a_Tank.AutonUpdate(0.5, -0.5);
		} else {
			a_Tank.AutonUpdate(0, 0);
			nextState = kMoveTowardsTower;
		}
		break;
	case kMoveTowardsTower:
		if (tankDistance < SHOOT_SPOT_DISTANCE) {
			a_Tank.AutonUpdate(0.5, 0.5);
		} else {
			a_Tank.AutonUpdate(0, 0);
			nextState = kLoadingBot;
		}
		break;
	case kLoadingBot:
		if (!a_CollectorSwitch.Get()) {
			a_Collector.Set(0.5);
		} else {
			a_Collector.Set(0.0);
			nextState = kLoaderDown;
		}
		break;
	case kLoaderDown:
		if (!a_CollectorSwitch.Get()) {
			a_Collector.Set(-0.5);
		} else {
			a_Collector.Set(0.0);
			nextState = kCheckAim;
		}
		break;
	case kCheckAim:
		// insert vision code here
		break;
	case kAdjust:
		// more vision code here- let's talk to Alexander!
		break;
	case kShoot:
		a_Shooter.Fire(); // unsure if cock function needs to be called // it doesn't, don't worry
		nextState = kTurnBack;
		break;
	case kTurnBack:
		if (a_Gyro.GetAngle() < TURN_AROUND_ANGLE) {
			a_Tank.AutonUpdate(0.5, -0.5);
		} else {
			a_Tank.AutonUpdate(0, 0);
			nextState = kDriveToTurnPoint;
		}
		break;
	case kDriveToTurnPoint:
		if (tankDistance < C_DISTANCE) {
			a_Tank.AutonUpdate(0.5, 0.5);
		} else {
			a_Tank.AutonUpdate(0, 0);
			nextState = kTurnToC;
		}
		break;
	case kTurnToC:
			if (a_Gyro.GetAngle() < TURN_TO_C_ANGLE) {
				a_Tank.AutonUpdate(0.5, -0.5);
			} else {
				a_Tank.AutonUpdate(0, 0);
				nextState = kDriveToC;
			}
			break;
	case kDriveToC:
			if (tankDistance < TO_C_DISTANCE) {
				a_Tank.AutonUpdate(0.5, 0.5);
			} else {
				a_Tank.AutonUpdate(0, 0);
				nextState = kAutoIdle;
			}
			break;
	case kAutoIdle:
		a_Tank.AutonUpdate(0, 0);
		break;
	}

	a_AutoState = nextState;

}

void SmokeyIX::TeleopInit()
{
	a_Gyro.Cal();
	a_Tank.Enable(); // Don't add mysterious methods in that we haven't tested, alexander
	a_Left.DisablePIDControl();
	a_Right.DisablePIDControl();
	a_Collector.Init();
	// a_TargetDetector.StartProcessing();
}

void SmokeyIX::TeleopPeriodic()
{
	float setAngle = SmartDashboard::GetNumber("Angle to set to", 0);
	a_Gyro.Update();
	a_Tank.Update(a_Joystick, a_Joystick2, a_Gyro.GetAngle(), setAngle);



	if(a_Joystick.GetRawButton(1)) {
		a_Shooter.Fire();
	}
	a_Shooter.Update(a_Joystick);

	/*
	if(a_Joystick.GetRawButton(2)) {
		a_Shooter.Fire();
	}

	a_Shooter.Update();
	*/

	a_Collector.Update(a_Joystick, 3, 5, 4, 6, 2, 0.5);


	if (a_Joystick.GetRawButton(4)) {
		a_Winch.Update(a_Joystick.GetRawButton(4));
	} else if (a_Joystick.GetRawButton(6)) {
		a_Winch.Update(-1.0 * a_Joystick.GetRawButton(6));
	} else {
		a_Winch.Update(0);
	}


	// a_Finger.Update(a_Joystick, 7, 8, 0.5);

	//Roller Test
	if(a_Joystick2.GetRawButton(2)) {
		a_Roller.Update(0.5 );
	} else if(a_Joystick2.GetRawButton(1)) {
		a_Roller.Update(-0.5);
	} else {
		a_Roller.Update(0);
	}

	if(a_Joystick.GetRawButton(2)) {
		a_Gyro.Zero();
	}

	SmartDashboard::PutNumber("Collector Angle", a_Collector.GetAngle());
	SmartDashboard::PutNumber("Gyro value", a_Gyro.GetAngle());
	SmartDashboard::PutNumber("Shooter", a_Shooter.GetPosition());
	SmartDashboard::PutNumber("Winch", a_Winch.GetLength());
	printf("Shooter angle value: %6.2f\n", a_Shooter.GetPosition());
	/* TODO: remove test code
	 SmartDashboard::PutBoolean("LeftA", a_LeftA.Get());
	 SmartDashboard::PutBoolean("RightB", a_RightB.Get());
	 SmartDashboard::PutBoolean("LeftB", a_LeftB.Get());
	 SmartDashboard::PutBoolean("RightA", a_RightA.Get());
	 */

	//Roller Test Code
}

void SmokeyIX::TestInit()
{
	a_Compressor.SetClosedLoopControl(true);
	a_LeftSol.Set(DoubleSolenoid::kForward);
}

void SmokeyIX::TestPeriodic()
{
	a_Tank.Update(a_Joystick, a_Joystick2, a_Gyro.GetAngle(), 0);

	if(a_Joystick.GetRawButton(1)) {
		a_Shooter.Set(a_Joystick.GetRawButton(1));
	} else {
		a_Shooter.Set(0);
	}


	// a_Collector.Update(a_Joystick, 3, 5, 0.5);

	if (a_Joystick.GetRawButton(4)) {
		a_Winch.Update(a_Joystick.GetRawButton(4));
	} else if (a_Joystick.GetRawButton(6)) {
		a_Winch.Update(-1.0 * a_Joystick.GetRawButton(6));
	} else {
		a_Winch.Update(0);
	}


	// a_Finger.Update(a_Joystick, 7, 8, 0.5);

	//Roller Test
	if(a_Joystick.GetRawButton(9)) {
		a_Roller.Update(1.0 );
	} else {
		a_Roller.Update(0);
	}


	SmartDashboard::PutNumber("Shooter", a_Shooter.GetPosition());
	SmartDashboard::PutNumber("Winch", a_Winch.GetLength());

}

START_ROBOT_CLASS(SmokeyIX);
