#pragma once
#include "ImageFilter.h"

class ThresholdFilter: public ImageFilter {
public:
	ThresholdFilter(ImageSource::Ptr src, int threshold);

	virtual ImgPtr GetImage();

protected:
	int a_Threshold;
};

class ClusterThresholdFilter: public ImageFilter {
public:
	ClusterThresholdFilter(ImageSource::Ptr src);
	virtual ImgPtr GetImage();
};
