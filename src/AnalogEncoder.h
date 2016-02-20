/*
 * AnalogEncoder.h
 *
 *  Created on: Feb 20, 2016
 *      Author: hstechclub
 */

#ifndef SRC_ANALOGENCODER_H_
#define SRC_ANALOGENCODER_H_

#include <WPILIb.h>

class AnalogEncoder
{
public:
	AnalogEncoder(int inputPort, int min, int max, int offset);
	~AnalogEncoder();
	float GetValue();
	void SetMin(int nMin);
	void SetMax(int nMax);
	void SetOffset(int nOffset);
private:
	AnalogInput a_Input;
	int a_Min;
	int a_Max;
	int a_Offset;
};



#endif /* SRC_ANALOGENCODER_H_ */
