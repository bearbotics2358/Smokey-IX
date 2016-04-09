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
		a_Winch(WINCH, WINCH_PORT_A, WINCH_PORT_B, WINCH_SWITCH_PORT, WINCH_SOL_PORT_ONE, WINCH_SOL_PORT_TWO),
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
		a_AutoState(kAutoIdle),
		a_SpyState(kSpyAutoIdle),
		a_TargetDetector("10.23.58.11")

		// a_Leonardo(BAUD_RATE, SerialPort::kUSB, DATA_BITS, SerialPort::kParity_None, SerialPort::kStopBits_One)
{
	// left encoder is running backwards
	a_Left.SetEncoderReverseDirection(true);
	tState = 0;
	shooterStart = -99;
	shooterCurrent = 1000000000;
	angleToT = 0;
	a_Winch.ShiftOpen();
	// a_Leonardo.Reset();
	// a_Leonardo.Write("B", 1);
}

void SmokeyIX::RobotInit()
{
	// a_Compressor.SetClosedLoopControl(true);
	a_LeftSol.Set(DoubleSolenoid::kForward);
	a_Winch.ShiftOpen();
	a_Tank.Init();
}

void SmokeyIX::DisabledInit()
{
	a_Tank.Disable();
	a_TargetDetector.StopProcessing();
	a_TargetDetector.TurnLRCDown();

}

void SmokeyIX::AutonomousInit()
{
	a_Gyro.Cal();
	a_AutoState = kDropCollector;
	a_SpyState = kSpyDropCollector;
	a_Tank.Enable();
	a_Collector.Init();
	a_Collector.SetAngle(90);
	a_Left.ResetEncoder();
	a_Right.ResetEncoder();
	tState = 0;
	a_Left.ShiftHigh();
	a_TargetDetector.TurnLRCBright();
	a_TargetDetector.StartProcessing();
}

void SmokeyIX::AutonomousPeriodic() {
	// AutonomousPeriodicSimple();
	AutonomousPeriodicFull();
}

void SmokeyIX::AutonomousPeriodicFull()
{

	AutoState nextState = a_AutoState;

	float tankDistance = a_Tank.GetDistance(); // getDistance() already converts to inches
	SmartDashboard::PutNumber("Tank Distance", tankDistance);

	const double LOW_BAR_DISTANCE = 77.0 - ROBOT_LENGTH - 23; // -23 for extra swag and drift control
	const double LOW_BAR_CLEAR = 48.0 + ROBOT_LENGTH;
	const double TURN_SPOT_DISTANCE = 113.06 - ROBOT_PIVOT_POINT;
	const double SHOOT_SPOT_DISTANCE = 130.9 - TOWER_DISTANCE;
	const double TURN_ANGLE = 60.0 - 5.0 + 1.0 - 1.0; // theoretically 60 degrees
	// float adjustAngle;
	const double TURN_AROUND_ANGLE =  TURN_ANGLE + (180 * M_1_PI) * asin(48.0/(sqrt(pow(SHOOT_SPOT_DISTANCE,2) - 96*sqrt(3)*SHOOT_SPOT_DISTANCE + 9216)));
	const double C_DISTANCE = (sqrt(pow(SHOOT_SPOT_DISTANCE,2) - 96*sqrt(3)*SHOOT_SPOT_DISTANCE + 9216));
	const double TURN_TO_C_ANGLE = 180.0; // check all these angles when testing
	const double TO_C_DISTANCE = TURN_SPOT_DISTANCE + ROBOT_PIVOT_POINT;

	a_Gyro.Update();
	float gyroValue = a_Gyro.GetAngle();
	SmartDashboard::PutNumber("Gyro, yum", gyroValue);

	a_Shooter.Update(a_Joystick);

	// printf("state %d\n", a_AutoState);

	switch (a_AutoState) {
	case kDropCollector:
		a_Collector.SetAngle(0);
		nextState = kMoveToLowBar;
		break;
	case kMoveToLowBar:
		if (tankDistance < LOW_BAR_DISTANCE) {
			a_Tank.AutonUpdateDriveStraight(-0.60, 0.60);
		} else {
			a_Tank.AutonUpdate(0, 0);
			nextState = kMoveUnderLowBar;
		}
		break;
	case kMoveUnderLowBar:
		if (tankDistance < LOW_BAR_CLEAR + LOW_BAR_DISTANCE) {
			a_Tank.AutonUpdateDriveStraight(-0.60, 0.60); //change to a usable speed
		} else {
			a_Tank.AutonUpdate(0, 0);
			nextState = kMoveToShoot;
		}
		break;
	case kMoveToShoot:
		a_Collector.SetAngle(120);
		if (tankDistance < TURN_SPOT_DISTANCE + LOW_BAR_CLEAR + LOW_BAR_DISTANCE) {
			if( tankDistance < (TURN_SPOT_DISTANCE + LOW_BAR_CLEAR + LOW_BAR_DISTANCE) - 24 ) {
				a_Tank.AutonUpdateDriveStraight(-0.50, 0.50);
			} else {
				a_Tank.AutonUpdateDriveStraight(-0.45, 0.45);
			}
		} else {
			a_Tank.AutonUpdate(0, 0);
			nextState = kTurnToShootWait;
			a_Tank.ResetEncoders();
			tState = Timer::GetFPGATimestamp();
		}
		break;
	case kTurnToShootWait:
		if(Timer::GetFPGATimestamp() >= tState + 0.5) {
			nextState = kTurnToShoot;
			a_Tank.SetTwistingMode();
			a_Tank.SetTwistingRelAngle(a_Gyro.GetAngle(), TURN_ANGLE); // Need to turn further in manual mode- 5 maybe? I think that is right
		}
		break;
	case kTurnToShoot:
		if (a_Tank.IsTwisting()) {
			a_Tank.Update(a_Joystick, a_Joystick2, a_Gyro.GetAngle());
		} else {
			a_Tank.AutonUpdate(0, 0);
			nextState = kMoveTowardsTowerWait;
			a_Tank.ResetEncoders();
			tState = Timer::GetFPGATimestamp();
		}
		break;
	case kMoveTowardsTowerWait:
		if(Timer::GetFPGATimestamp() >= tState + 0.75) {
			nextState = kMoveTowardsTower;
			a_Tank.ResetEncoders();
		}
		break;
	case kMoveTowardsTower:
		if (tankDistance < SHOOT_SPOT_DISTANCE) {
			a_Tank.AutonUpdateDriveStraight(-0.45, 0.45);
		} else {
			a_Tank.AutonUpdate(0, 0);
			nextState = kLoadingBot;
			a_Tank.ResetEncoders();
		}
		break;
	case kLoadingBot:
			a_Collector.SetAngle(120.0);
			if( fabs(a_Collector.GetAngle() - 120.0) < 3) {
				nextState = kLoaderDownWait;
				a_Tank.ResetEncoders();
				tState = Timer::GetFPGATimestamp();
			}
		break;
	case kLoaderDownWait:
		a_Roller.Update(-0.25);
		if(Timer::GetFPGATimestamp() >= tState + 1.0) {
			nextState = kLoaderDown;
			a_Tank.ResetEncoders();
		}
		break;
	case kLoaderDown:
		a_Roller.Update(0);
		a_Collector.SetAngle(0);
		if( fabs(a_Collector.GetAngle()) < 3) {
			nextState = kShootWait;
			a_Tank.ResetEncoders();
			tState = Timer::GetFPGATimestamp();
		}
		break;
	case kCheckAimWait:
		if(Timer::GetFPGATimestamp() >= tState + 1.2) {
			nextState = kCheckAim;
			a_Tank.ResetEncoders();
		}
		break;
	case kCheckAim:
		a_Tank.SetTwistingMode();

		angleToT = a_TargetDetector.GetAngleToTarget();
		if(fabs(angleToT > 15)) {
			nextState = kShootWait;
			tState = Timer::GetFPGATimestamp();
			break;
		}

		if(a_TargetDetector.CanSeeTarget() && angleToT >= 0) {
			a_Tank.SetTwistingRelAngle(a_Gyro.GetAngle(), a_TargetDetector.GetAngleToTarget() + 3);
			printf("attempting to turn to %f\n", a_TargetDetector.GetAngleToTarget());
		} else if(a_TargetDetector.CanSeeTarget() && angleToT < 0) {
			a_Tank.SetTwistingRelAngle(a_Gyro.GetAngle(), a_TargetDetector.GetAngleToTarget() - 3);
			printf("attempting to turn to %f\n", a_TargetDetector.GetAngleToTarget());
		} else {
			a_Tank.SetTwistingRelAngle(a_Gyro.GetAngle(), 0);
			printf("no target. woohoo.\n");
		}

		nextState = kAdjust;
		break;
	case kAdjust:
		if(a_Tank.IsTwisting()) {
			a_Tank.Update(a_Joystick, a_Joystick2, a_Gyro.GetAngle());
		} else {
			nextState = kShootWait;
			tState = Timer::GetFPGATimestamp();
		}

		break;
	case kShootWait:
		if(Timer::GetFPGATimestamp() >= tState + 1.0) {
			nextState = kShoot;
			shooterStart = a_Shooter.GetPosition();
		}
		break;
	case kShoot:
		a_Shooter.Fire(); // unsure if cock function needs to be called // it doesn't, don't worry
		nextState = kTurnWait;
		a_Tank.ResetEncoders();
		break;
	case kTurnWait:
		shooterCurrent = a_Shooter.GetPosition();
			if(shooterCurrent < shooterStart) {
				nextState = kAutoIdle;
				a_Tank.SetTwistingMode();
				a_Tank.SetTwistingRelAngle(a_Gyro.GetAngle(), TURN_AROUND_ANGLE - 5);
			} else {
				a_Shooter.Fire();
			}
			break;
	case kTurnBack:
		if (a_Tank.IsTwisting()) {
			a_Tank.Update(a_Joystick, a_Joystick2, a_Gyro.GetAngle());
		} else {
			a_Tank.AutonUpdate(0, 0);
			nextState = kDriveToTurnPoint;
			a_Tank.ResetEncoders();
		}
		break;
	case kDriveToTurnPoint:
		if (tankDistance < C_DISTANCE) {
			a_Tank.AutonUpdateDriveStraight(-0.35, 0.35);
		} else {
			a_Tank.AutonUpdate(0, 0);
			nextState = kTurnToC;
			a_Tank.ResetEncoders();
			a_Tank.SetTwistingMode();
			a_Tank.SetTwistingRelAngle(a_Gyro.GetAngle(), TURN_TO_C_ANGLE - 10);
		}
		break;
	case kTurnToC:
			if (a_Tank.IsTwisting()) {
				a_Tank.Update(a_Joystick, a_Joystick2, a_Gyro.GetAngle());
			} else {
				a_Tank.AutonUpdate(0, 0);
				nextState = kDriveToC;
				a_Tank.ResetEncoders();
			}
			break;
	case kDriveToC:
			if (tankDistance < TO_C_DISTANCE) {
				a_Tank.AutonUpdateDriveStraight(-0.35, 0.35);
			} else {
				a_Tank.AutonUpdate(0, 0);
				nextState = kAutoIdle;
				a_Tank.ResetEncoders();
			}
			break;
	case kAutoIdle:
		a_Tank.AutonUpdate(0, 0);
		a_Tank.ResetEncoders();
		break;
	}

	a_AutoState = nextState;
}

void SmokeyIX::AutonomousPeriodicSimple() {
	spyAutoState SnextState = a_SpyState;

	a_Shooter.Update(a_Joystick);

	switch (a_SpyState) {
	case kSpyDropCollector:
		a_Collector.SetAngle(0);
		SnextState = kSpyDropCollectorWait;
		break;
	case kSpyDropCollectorWait:
		if( fabs(a_Collector.GetAngle()) < 3) {
			SnextState = kSpyLoadingBot;
		}
		break;
	case kSpyLoadingBot:
		a_Collector.SetAngle(120.0);
		if( fabs(a_Collector.GetAngle() - 120.0) < 3) {
			SnextState = kSpyLoadingDownWait;
			tState = Timer::GetFPGATimestamp();
		}
		break;
	case kSpyLoadingDownWait:
		if(Timer::GetFPGATimestamp() >= tState + 0.5) {
			SnextState = kSpyLoaderDown;
		}
		break;
	case kSpyLoaderDown:
		a_Collector.SetAngle(0);
		if( fabs(a_Collector.GetAngle()) < 3) {
			SnextState = kSpyShootWait;
			tState = Timer::GetFPGATimestamp();
		}
		break;
	case kSpyShootWait:
		if(Timer::GetFPGATimestamp() >= tState + 2.0) {
			SnextState = kSpyShoot;
			shooterStart = a_Shooter.GetPosition();
		}
		break;
	case kSpyShoot:
		shooterCurrent = a_Shooter.GetPosition();
		if(shooterCurrent < shooterStart) {
			SnextState = kSpyAutoIdle;
		} else {
			a_Shooter.Fire();
		}
		break;
	case kSpyAutoIdle:
		a_Tank.AutonUpdate(0, 0);
		a_Tank.ResetEncoders();
		break;
	}

	a_SpyState = SnextState;
}

void SmokeyIX::TeleopInit()
{
	a_Gyro.Cal();
	a_Left.ResetEncoder();
	a_Right.ResetEncoder();
	a_Tank.Enable(); // Don't add mysterious methods in that we haven't tested, alexander
	a_Left.DisablePIDControl();
	a_Right.DisablePIDControl();
	a_Collector.Init();
	a_Collector.SetAngle(120);
	a_TargetDetector.TurnLRCBright();
	a_TargetDetector.StartProcessing();
	a_Tank.DisableTwist();
	a_Winch.ShiftOpen();

}

void SmokeyIX::TeleopPeriodic()
{
	double angleToTarget = a_TargetDetector.GetAngleToTarget();
	SmartDashboard::PutNumber("Angle to Target", angleToTarget);

	a_Gyro.Update();

	if(a_Joystick2.GetRawButton(6) && a_TargetDetector.CanSeeTarget()) {

		a_Tank.SetTwistingMode();
		a_Tank.SetTwistingRelAngle(a_Gyro.GetAngle(), angleToTarget);
	}

	if(a_Joystick2.GetPOV() == 90) {
		a_Tank.SetTwistingMode();
		a_Tank.SetTwistingRelAngle(a_Gyro.GetAngle(), 4.5);
	} else if(a_Joystick2.GetPOV() == 270) {
		a_Tank.SetTwistingMode();
		a_Tank.SetTwistingRelAngle(a_Gyro.GetAngle(), -4.5);
	}

	a_Tank.Update(a_Joystick, a_Joystick2, a_Gyro.GetAngle());


	if(a_Joystick2.GetRawButton(1)) {
		a_Shooter.Fire();
	}
	if(a_Joystick2.GetRawButton(2)) {
		a_Shooter.Stop();
	}
	a_Shooter.Update(a_Joystick);

	/*
	if(a_Joystick.GetRawButton(2)) {
		a_Shooter.Fire();
	}

	a_Shooter.Update();
	*/

	a_Collector.Update(a_Joystick, 3, 5, 4, 6, 2);

	/*
	if (a_Joystick2.GetRawButton(4)) {
		a_Winch.Set(a_Joystick2.GetRawButton(4));
	} else if (a_Joystick2.GetRawButton(5)) {
		a_Winch.Set(-1.0 * a_Joystick2.GetRawButton(5));
	} else {
		a_Winch.Set(0);
	}
	*/

	if(a_Joystick.GetRawButton(9)) {
		a_Winch.ShiftClosed();
	} else if(a_Joystick.GetRawButton(10)) {
		a_Winch.ShiftOpen();
	}

	if(a_Joystick2.GetRawButton(5)) {
		a_Winch.Set(a_Joystick2.GetRawAxis(4));
	}


	// a_Finger.Update(a_Joystick, 7, 8, 0.5);

	//Roller Test
	if(a_Joystick.GetRawButton(8)) {
		a_Roller.Update(0.25);
	} else if(a_Joystick.GetRawButton(1)) {
		a_Roller.Update(-0.5);
	} else {
		a_Roller.Update(0);
	}

	if(a_Joystick2.GetRawButton(3)) {
		a_Gyro.Cal();
	}


	SmartDashboard::PutNumber("Collector Angle", a_Collector.GetAngle());
	SmartDashboard::PutNumber("Gyro value", a_Gyro.GetAngle());
	SmartDashboard::PutNumber("Shooter", a_Shooter.GetPosition());
	SmartDashboard::PutNumber("Winch", a_Winch.GetLength());

	/*
	SmartDashboard::PutNumber("Current left 1", a_PDP.GetCurrent(BACK_LEFT_ONE - 10));
	SmartDashboard::PutNumber("Current left 2", a_PDP.GetCurrent(BACK_LEFT_TWO - 10));
	SmartDashboard::PutNumber("Current right 1", a_PDP.GetCurrent(BACK_RIGHT_ONE - 10));
	SmartDashboard::PutNumber("Current right 2", a_PDP.GetCurrent(BACK_RIGHT_TWO - 10));
	*/

	// printf("Shooter angle value: %6.2f\n", a_Shooter.GetPosition());
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
	a_Tank.Enable();
	a_Tank.DisableTwist();
	a_TargetDetector.TurnLRCBright();
}

void SmokeyIX::TestPeriodic()
{
	a_Tank.AutonUpdate(-0.35, 0.35);
	return;



	a_Tank.Update(a_Joystick, a_Joystick2, a_Gyro.GetAngle());

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
		a_Roller.Update(1.0);
	} else {
		a_Roller.Update(0);
	}


	SmartDashboard::PutNumber("Shooter", a_Shooter.GetPosition());
	SmartDashboard::PutNumber("Winch", a_Winch.GetLength());

}

START_ROBOT_CLASS(SmokeyIX);
