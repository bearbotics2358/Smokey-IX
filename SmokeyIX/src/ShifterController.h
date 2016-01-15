#pragma once


#include "SpeedController.h"
#include "PIDOutput.h"
#include "PWM.h"
#include "Victor.h"
#include <WPILib.h>

class ShifterController :  public SpeedController
{
public:
	ShifterController(Victor &Left, Victor &Right, DoubleSolenoid &Shift);
	virtual ~ShifterController();
	virtual void Set(float value, uint8_t syncGroup = 0);
	virtual void Shift();
	virtual float Get();
	virtual void Disable();

	virtual void PIDWrite(float output);

private:
	Victor &LeftC;
	Victor &RightC;
	DoubleSolenoid &Sol;

	float m_speed;
};
