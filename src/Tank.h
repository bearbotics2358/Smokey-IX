#pragma once

#include <WPILib.h>
#include "Prefs.h"
#include "ShifterController.h"

class Tank {
public:
	Tank(ShifterController &Left, ShifterController &Right);
	~Tank();

	void Init();

	void Enable();
	void Disable();

	void Update(Joystick &stick, Joystick &stick2);
	void AutonUpdate(double left, double right);
	float GetDistance();

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

	static const std::string ARCADE_TUNING_PARAM_A_KEY;
	static const double      ARCADE_TUNING_PARAM_A_DEFAULT;
	static const std::string ARCADE_TUNING_PARAM_B_KEY;
	static const double      ARCADE_TUNING_PARAM_B_DEFAULT;

	ShifterController &a_LeftSide;
	ShifterController &a_RightSide;

	SendableChooser a_ControlTypeChooser;
};
