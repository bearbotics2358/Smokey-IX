#include "WPILib.h"
#include "SmokeyIX.h"
#include "Prefs.h"

SmokeyIX::SmokeyIX(void)
: a_Joystick(JOYSTICK_PORT),
  a_Joystick2(JOYSTICKTWO_PORT),
  a_BLOne(BACK_LEFT_ONE),
  a_BLTwo(BACK_LEFT_TWO),
  a_BROne(BACK_RIGHT_ONE),
  a_BRTwo(BACK_RIGHT_TWO),
  a_WinchOne(WINCH_ONE),
  a_WinchTwo(WINCH_TWO),
  a_CollectorOne(COLLECTOR_ONE),
  a_CollectorTwo(COLLECTOR_TWO),
  a_Roller(ROLLER),
  a_LeftSol(LEFT_SOL_PORT_ONE, LEFT_SOL_PORT_TWO),
  a_RightSol(RIGHT_SOL_PORT_ONE, RIGHT_SOL_PORT_TWO),
  a_Left(a_BLOne, a_BLTwo, a_LeftSol),
  a_Right(a_BROne, a_BRTwo, a_RightSol),
  a_Tank(a_Left, a_Right)
{

}

void SmokeyIX::RobotInit()
{

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

}

void SmokeyIX::TestPeriodic()
{
	a_Tank.Update(a_Joystick, a_Joystick2);
}

START_ROBOT_CLASS(SmokeyIX);
