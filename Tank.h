#pragma once

#include <WPIlib.h>
#include "Prefs.h"
#include "ShifterController.h"


class Tank {
public:
	Tank(ShifterController &Left, ShifterController &Right);
	virtual ~Tank();
	void Update(Joystick &stick, Joystick &stick2);

private:
	ShifterController a_LeftSide;
	ShifterController a_RightSide;
};
