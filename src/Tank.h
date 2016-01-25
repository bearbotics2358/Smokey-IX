#pragma once

#include <WPILib.h>
#include "Prefs.h"
#include "ShifterController.h"

class Tank {
public:
	Tank(ShifterController &Left, ShifterController &Right);
	~Tank();
	void Update(Joystick &stick, Joystick &stick2);

private:
	ShifterController &a_LeftSide;
	ShifterController &a_RightSide;
};
