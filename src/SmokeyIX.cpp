#include "WPILib.h"
#include "SmokeyIX.h"
#include "Prefs.h"
#include "PowerDistributionPanel.h"

SmokeyIX::SmokeyIX(void) :
a_Joystick(JOYSTICK_PORT),
a_Joystick2(JOYSTICKTWO_PORT),
a_PDP(PDP_PORT),
a_Compressor(PCM_PORT),
a_BLOne(BACK_LEFT_ONE),
a_BLTwo(BACK_LEFT_TWO),
a_BROne(BACK_RIGHT_ONE),
a_BRTwo(BACK_RIGHT_TWO),
a_LeftEncoder(LEFT_ENCODER_PORT_A,LEFT_ENCODER_PORT_B),
a_RightEncoder(RIGHT_ENCODER_PORT_A,RIGHT_ENCODER_PORT_B),
a_Winch(WINCH, WINCH_PORT_A,WINCH_PORT_B),
a_Finger(FINGER, FINGER_ENCODER_PORT, 0, 0), // Third argument is our upper limit on the encoder, fourth is our lower limit
a_Collector(COLLECTOR, COLLECTOR_ENCODER_PORT, 0, 0), // See above
a_Shooter(SHOOTER, SHOOTER_ENCODER_PORT),
a_Roller(ROLLER,ROLLER_SWITCH_PORT),
a_LeftSol(PCM_PORT, LEFT_SOL_PORT_ONE,LEFT_SOL_PORT_TWO),   // Must specify port # if not 0
a_Gyro(I2C::kMXP), // Didn't work because we used smartdashboard in the constructor- wait to use it until after RobotInit()
a_Left(a_BLOne, a_BLTwo, a_LeftSol, LEFT_ENCODER_PORT_A,LEFT_ENCODER_PORT_B),
a_Right(a_BROne, a_BRTwo, a_LeftSol,RIGHT_ENCODER_PORT_A, RIGHT_ENCODER_PORT_B),
a_Tank(a_Left,a_Right),
a_AutoState(kAutoIdle){
}

void SmokeyIX::RobotInit() {
	// a_Compressor.SetClosedLoopControl(true);
	a_LeftSol.Set(DoubleSolenoid::kForward);
	a_Tank.Init();
}

void SmokeyIX::AutonomousInit() {
	a_Gyro.Cal();
	a_AutoState = kMoveUnderLowBar;
}

void SmokeyIX::AutonomousPeriodic() {
	AutoState nextState = a_AutoState;

	int tankDistance = 0;

	const int lowBarDistance = 0;
	const int lowBarClear = 0;
	const int shootSpotDistance = 0;
	const double turnAngle = 0.0;	const double turnAroundAngle = 0.0;
	const int cDistance = 0;

	switch (a_AutoState) {
	case kMoveToLowBar:
		if (tankDistance < lowBarDistance) {
			a_Tank.AutonUpdate(0.5, 0.5);
		} else {
			a_Tank.AutonUpdate(0, 0);
			nextState = kMoveUnderLowBar;
		}
		break;
	case kMoveUnderLowBar:
		if (tankDistance < lowBarClear) {
			a_Tank.AutonUpdate(0.5, 0.5); //change to a usable speed
		} else {
			a_Tank.AutonUpdate(0, 0);
			nextState = kMovePerp;
		}
		break;
	case kMovePerp:
		if (tankDistance < shootSpotDistance) {
			a_Tank.AutonUpdate(0.5, 0.5);
		} else {
			a_Tank.AutonUpdate(0, 0);
			nextState = kTurnPerp;
		}
		break;
	case kTurnPerp:
		if (a_Gyro.GetAngle() < turnAngle) {
			a_Tank.AutonUpdate(0.5, -0.5);
		} else {
			a_Tank.AutonUpdate(0, 0);
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
		a_Shooter.Fire(); // unsure if cock function needs to be called
		nextState = kTurnBack;
		break;
	case kTurnBack:
		if (a_Gyro.GetAngle() < turnAroundAngle) {
			a_Tank.AutonUpdate(0.5, -0.5);
		} else {
			a_Tank.AutonUpdate(0, 0);
			nextState = kDriveToC;
		}
		break;
	case kDriveToC:
		if (tankDistance < cDistance) {
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

void SmokeyIX::TeleopInit() {
	a_Gyro.Cal();
}

void SmokeyIX::TeleopPeriodic() {
	a_Tank.Update(a_Joystick, a_Joystick2);
	a_Gyro.Update();
	SmartDashboard::PutNumber("Left Encoder (Distance)",
			a_LeftEncoder.GetDistance());
	SmartDashboard::PutNumber("Left Encoder (Raw)", a_LeftEncoder.GetRaw());
	SmartDashboard::PutNumber("Right Encoder (Distance)",
			a_RightEncoder.GetDistance());
	SmartDashboard::PutNumber("Right Encoder (Raw)", a_RightEncoder.GetRaw());
	SmartDashboard::PutNumber("Gyro value", a_Gyro.GetAngle());

	/* TODO: remove test code
	 SmartDashboard::PutBoolean("LeftA", a_LeftA.Get());
	 SmartDashboard::PutBoolean("RightB", a_RightB.Get());
	 SmartDashboard::PutBoolean("LeftB", a_LeftB.Get());
	 SmartDashboard::PutBoolean("RightA", a_RightA.Get());
	 */

	//Roller Test Code
}

void SmokeyIX::TestInit() {
	a_Compressor.SetClosedLoopControl(true);
	a_LeftSol.Set(DoubleSolenoid::kForward);
}

void SmokeyIX::TestPeriodic() {
	a_Tank.Update(a_Joystick, a_Joystick2);

	a_Shooter.Set(a_Joystick.GetRawButton(1));

	a_Collector.Update(a_Joystick, 3, 5, 0.5);

	if (a_Joystick.GetRawButton(4)) {
		a_Winch.Update(a_Joystick.GetRawButton(4));
	} else if (a_Joystick.GetRawButton(6)) {
		a_Winch.Update(-1.0 * a_Joystick.GetRawButton(6));
	}

	a_Finger.Update(a_Joystick, 7, 8, 0.5);

	//Roller Test
	a_Roller.Update(-1.0 * a_Joystick.GetRawButton(9));

	SmartDashboard::PutNumber("Left", a_LeftEncoder.GetDistance());
	SmartDashboard::PutNumber("Right", a_RightEncoder.GetDistance());

	/* TODO: remove test code
	 SmartDashboard::PutNumber("LeftA", a_LeftA.Get());
	 SmartDashboard::PutNumber("RightB", a_RightB.Get());
	 SmartDashboard::PutNumber("LeftB", a_LeftB.Get());
	 SmartDashboard::PutNumber("RightA", a_RightA.Get());
	 */

	SmartDashboard::PutNumber("Current A", a_PDP.GetCurrent(0)); // Not actually errors- Eclipse is just mad
	SmartDashboard::PutNumber("Current B", a_PDP.GetCurrent(1));
	SmartDashboard::PutNumber("Current C", a_PDP.GetCurrent(2));
	SmartDashboard::PutNumber("Current D", a_PDP.GetCurrent(3));
}

START_ROBOT_CLASS(SmokeyIX);
