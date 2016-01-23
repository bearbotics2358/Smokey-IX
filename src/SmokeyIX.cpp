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
  a_WinchOne(WINCH_ONE),
  a_WinchTwo(WINCH_TWO),
  a_CollectorOne(COLLECTOR_ONE),
  a_CollectorTwo(COLLECTOR_TWO),
  a_Roller(ROLLER),
  a_LeftSol(PCM_PORT, LEFT_SOL_PORT_ONE, LEFT_SOL_PORT_TWO),   // Must specify port # if not 0
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
	a_Tank.Update(a_Joystick,a_Joystick2);
}

void SmokeyIX::TestInit()
{
	a_Compressor.SetClosedLoopControl(true);
	a_LeftSol.Set(DoubleSolenoid::kForward);
}

void SmokeyIX::TestPeriodic()
{
	a_Tank.Update(a_Joystick, a_Joystick2);

	SmartDashboard::PutNumber("Current A", a_PDP.GetCurrent(0)); // Not actually errors- Eclipse is just mad
	SmartDashboard::PutNumber("Current B", a_PDP.GetCurrent(1));
	SmartDashboard::PutNumber("Current C", a_PDP.GetCurrent(2));
	SmartDashboard::PutNumber("Current D", a_PDP.GetCurrent(3));
}

START_ROBOT_CLASS(SmokeyIX);
