/*
 * Roller.h
 *
 *  Created on: Feb 3, 2016
 *      Author: hstechclub
 */

#ifndef SRC_ROLLER_H_
#define SRC_ROLLER_H_

class Roller // This is for an arm that pivots, but never goes a full 360- uses an Absolute Encoder
{
public:
	Roller(int TalonPort, int SwitchPort);
	~Roller();
	void Update(float value, uint8_t syncGroup = 0);
	bool GetSwitch();
	void Disable();

private:
	CanTalonSRX ArmC;

	DigitalInput SwitchC;
};



#endif /* SRC_ROLLER_H_ */
