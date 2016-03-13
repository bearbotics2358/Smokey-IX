#pragma once

#include "Prefs.h"
#include "ShifterController.h"
#include <WPILib.h>

class Tank {
public:
	Tank(ShifterController &Left, ShifterController &Right);
	~Tank();

	void Init();

	void Enable();
	void Disable();

	void Update(Joystick &stick, Joystick &stick2, float gyroValue);
	void AutonUpdate(double left, double right);
	void AutonUpdateDriveStraightTest(double left, double right, Joystick &stick);
	void AutonUpdateDriveStraight(double left, double right);
	void SimpleUpdate(Joystick &stick, Joystick &stick2);
	void SetTwistingMode();
	void DisableTwist();
	void SetTwistingAngle(float angle);
	void SetTwistingRelAngle(float gyroAngle, float angle);
	float GetDistance();
	void ResetEncoders();
	bool IsTwisting();

private:
	static const std::string CONTROL_TYPE_KEY;

	static const int         CONTROL_TYPE_TANK_TWO_JOYSTICKS;
	static const std::string CONTROL_TYPE_TANK_TWO_JOYSTICKS_KEY;
	static const int         CONTROL_TYPE_TANK_GAMEPAD;
	static const std::string CONTROL_TYPE_TANK_GAMEPAD_KEY;
	static const int         CONTROL_TYPE_ARCADE_ONE_JOYSTICK;
	static const std::string CONTROL_TYPE_ARCADE_ONE_JOYSTICK_KEY;
	static const int         CONTROL_TYPE_ARCADE_ONE_GAMEPAD_STICK;
	static const std::string CONTROL_TYPE_ARCADE_ONE_GAMEPAD_STICK_KEY;
	static const int         CONTROL_TYPE_ARCADE_TWO_GAMEPAD_STICKS;
	static const std::string CONTROL_TYPE_ARCADE_TWO_GAMEPAD_STICKS_KEY;
	static const int		 CONTROL_TYPE_TWIST;
	static const std::string CONTROL_TYPE_TWIST_KEY;
	static const int		 CONTROL_TYPE_STICK_TWIST;
	static const std::string CONTROL_TYPE_STICK_TWIST_KEY;

	static const std::string ARCADE_TUNING_PARAM_A_KEY;
	static const double      ARCADE_TUNING_PARAM_A_DEFAULT;
	static const std::string ARCADE_TUNING_PARAM_B_KEY;
	static const double      ARCADE_TUNING_PARAM_B_DEFAULT;

	const double kJoystickDeadzone = 0.08;

	ShifterController &a_LeftSide;
	ShifterController &a_RightSide;

	SendableChooser a_ControlTypeChooser;
};
