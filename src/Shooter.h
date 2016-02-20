/*
 * Shooter.h
 *
 *  Created on: Feb 4, 2016
 *      Author: hstechclub
 */

#include "CanTalonSRX.h"
#include <WPILib.h>

#ifndef SRC_SHOOTER_H_
#define SRC_SHOOTER_H_

class Shooter // Motor that drives in one direction, sort of like a train- uses absolute encoder
{
public:

	Shooter(int TalonPort, int AbsPort);
	~Shooter();
	void Update(Joystick &a_Stick);
	float GetPosition();
	void SetPosition(float SetPoint);
	void Cock();
	void Fire();
	void Set(float value);
	void Disable();

private:

	CanTalonSRX ShooterC;

	AnalogInput EncoderC;
};



#endif /* SRC_SHOOTER_H_ */
