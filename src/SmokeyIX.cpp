#include "WPILib.h"
#include "SmokeyIX.h"
#include "Prefs.h"
#include "PowerDistributionPanel.h"

SmokeyIX::SmokeyIX(void)
: a_Joystick(JOYSTICK_PORT),
  a_Joystick2(JOYSTICKTWO_PORT),
  a_PDP(PDP_PORT),
  a_Compressor(PCM_PORT),
  a_BLOne(BACK_LEFT_ONE),
  a_BLTwo(BACK_LEFT_TWO),
  a_BROne(BACK_RIGHT_ONE),
  a_BRTwo(BACK_RIGHT_TWO),
  a_LeftEncoder(LEFT_ENCODER_PORT_A, LEFT_ENCODER_PORT_B),
  a_RightEncoder(RIGHT_ENCODER_PORT_A, RIGHT_ENCODER_PORT_B),
  /* TODO: remove test code
  a_LeftA(LEFT_ENCODER_PORT_A),
  a_LeftB(LEFT_ENCODER_PORT_B),
  a_RightA(RIGHT_ENCODER_PORT_A),
  a_RightB(RIGHT_ENCODER_PORT_B),
  */
  a_Winch(WINCH),
  a_Finger(FINGER),
  a_Collector(COLLECTOR),
  a_Shooter(SHOOTER),
  a_Roller(ROLLER),
  a_LeftSol(PCM_PORT, LEFT_SOL_PORT_ONE, LEFT_SOL_PORT_TWO),   // Must specify port # if not 0
  a_Gyro(I2C::kMXP), // something about this particular statements makes the robot not accept code
  a_Left(a_BLOne, a_BLTwo, a_LeftSol),
  a_Right(a_BROne, a_BRTwo, a_LeftSol),
  a_Tank(a_Left, a_Right)
{

}

void SmokeyIX::RobotInit()
{
	// a_Compressor.SetClosedLoopControl(true);
	a_LeftSol.Set(DoubleSolenoid::kForward);
	a_Tank.Init();
}

void SmokeyIX::AutonomousInit()
{
	a_Gyro.Cal();

}

void SmokeyIX::AutonomousPeriodic()
{
	//Backwards
	//a_Left.Set(0.5);
	//a_Right.Set(-0.5);

	a_Left.Set(-0.5);
	a_Right.Set(0.5);

}

void SmokeyIX::TeleopInit()
{
	a_Gyro.Cal();
}

void SmokeyIX::TeleopPeriodic()
{
	a_Tank.Update(a_Joystick,a_Joystick2);
	a_Gyro.Update();
	SmartDashboard::PutNumber("Left Encoder (Distance)", a_LeftEncoder.GetDistance());
	SmartDashboard::PutNumber("Left Encoder (Raw)", a_LeftEncoder.GetRaw());
	SmartDashboard::PutNumber("Right Encoder (Distance)", a_RightEncoder.GetDistance());
	SmartDashboard::PutNumber("Right Encoder (Raw)", a_RightEncoder.GetRaw());
	SmartDashboard::PutNumber("Gyro value", a_Gyro.GetAngle());

	/* TODO: remove test code
	SmartDashboard::PutBoolean("LeftA", a_LeftA.Get());
	SmartDashboard::PutBoolean("RightB", a_RightB.Get());
	SmartDashboard::PutBoolean("LeftB", a_LeftB.Get());
	SmartDashboard::PutBoolean("RightA", a_RightA.Get());
	*/
}

void SmokeyIX::TestInit()
{
	a_Compressor.SetClosedLoopControl(true);
	a_LeftSol.Set(DoubleSolenoid::kForward);
}

void SmokeyIX::TestPeriodic()
{
	a_Tank.Update(a_Joystick, a_Joystick2);
	a_Shooter.Set(a_Joystick.GetRawButton(1));
	a_Collector.Set(a_Joystick.GetRawButton(5));
	a_Collector.Set(-1.0 * a_Joystick.GetRawButton(3));
	a_Winch.Set(a_Joystick.GetRawButton(4));
	a_Winch.Set(-1.0 * a_Joystick.GetRawButton(6));
	a_Finger.Set(a_Joystick.GetRawButton(7));
	a_Finger.Set(-1.0 * a_Joystick.GetRawButton(8));

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
