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
  a_Winch(WINCH),
  a_Finger(FINGER),
  a_Collector(COLLECTOR),
  a_Shooter(SHOOTER),
  a_Roller(ROLLER),
  a_LeftSol(PCM_PORT, LEFT_SOL_PORT_ONE, LEFT_SOL_PORT_TWO),   // Must specify port # if not 0
  // a_Gyro(I2C::kMXP), // something about this particular statements makes the robot not accept code
  a_Left(a_BLOne, a_BLTwo, a_LeftSol),
  a_Right(a_BROne, a_BRTwo, a_LeftSol),
  a_Tank(a_Left, a_Right)
{

}

void SmokeyIX::RobotInit()
{
	// a_Compressor.SetClosedLoopControl(true);
	a_LeftSol.Set(DoubleSolenoid::kForward);
}

void SmokeyIX::AutonomousInit()
{

}

void SmokeyIX::AutonomousPeriodic()
{

}

void SmokeyIX::TeleopInit()
{

}

void SmokeyIX::TeleopPeriodic()
{
	// a_Tank.Update(a_Joystick,a_Joystick2);
	// a_Gyro.Update();
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


	SmartDashboard::PutNumber("Current A", a_PDP.GetCurrent(0)); // Not actually errors- Eclipse is just mad
	SmartDashboard::PutNumber("Current B", a_PDP.GetCurrent(1));
	SmartDashboard::PutNumber("Current C", a_PDP.GetCurrent(2));
	SmartDashboard::PutNumber("Current D", a_PDP.GetCurrent(3));
}

START_ROBOT_CLASS(SmokeyIX);
