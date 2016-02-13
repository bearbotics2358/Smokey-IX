#pragma once
#include "ImageFilter.h"

class ThresholdFilter: public ImageFilter {
public:
	ThresholdFilter(ImgSrcRef src, int threshold);

	virtual ImgRef GetImage();

protected:
	int a_Threshold;
};
