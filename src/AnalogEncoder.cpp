/*
 * AnalogEncoder.cpp
 *
 *  Created on: Feb 20, 2016
 *      Author: hstechclub
 */
#include "AnalogEncoder.h"

AnalogEncoder::AnalogEncoder(int inputPort, int min, int max, int offset)
: a_Input(inputPort)

{
	a_Min = min;
	a_Max = max;
	a_Offset = offset;
}

AnalogEncoder::~AnalogEncoder()
{

}

float AnalogEncoder::GetValue()
{
	return ((a_Input.GetValue() * a_Max) / 4095) + a_Min ;
}

void AnalogEncoder::SetMin(int nMin)
{
	a_Min = nMin;
}

void AnalogEncoder::SetMax(int nMax)
{
	a_Max = nMax;
}

void AnalogEncoder::SetOffset(int nOffset)
{
	a_Offset = nOffset;
}


