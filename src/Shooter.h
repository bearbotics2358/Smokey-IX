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
	void Update();
	float GetPosition();
	void SetPosition(float SetPoint);
	void Cock();
	void Fire();
	void Set(int value);
	void Disable();

private:

	CanTalonSRX ShooterC;

	DigitalInput EncoderC;
};



#endif /* SRC_SHOOTER_H_ */
