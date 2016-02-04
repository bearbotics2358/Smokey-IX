/*
 * Roller.h
 *
 *  Created on: Feb 3, 2016
 *      Author: hstechclub
 */

#ifndef SRC_ROLLER_H_
#define SRC_ROLLER_H_

class Roller // This is for a roller motor- uses a switch to detect if a gamepiece is currently in the assembly
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
