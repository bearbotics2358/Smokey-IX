/*
 * PivotArm.h
 *
 *  Created on: Feb 3, 2016
 *      Author: hstechclub
 */

#include <WPILib.h>
#include <PIDController.h>

#ifndef SRC_PIVOTARM_H_
#define SRC_PIVOTARM_H_

class PivotArm // This is for an arm that pivots, but never goes a full 360- uses an Absolute Encoder
{
public:
	PivotArm(int TalonPort, int AbsPort, int Up, int Low);
	~PivotArm();
	void Init();
	void Set(float value, uint8_t syncGroup = 0);
	void Update(Joystick &stick, int port1, int port2, float value);
	float GetAngle();
	void Disable();

	void PIDWrite(float output);

private:
	int upperLimit;
	int lowerLimit;

	CanTalonSRX ArmC;

	AnalogInput EncoderC;

};



#endif /* SRC_PIVOTARM_H_ */
