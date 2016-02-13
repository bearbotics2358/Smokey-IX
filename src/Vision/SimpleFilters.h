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

class ParticleSizeFilter: public ImageFilter {
public:
	enum FilterType {
		KEEP_SMALL_PARTICLES,
		KEEP_LARGE_PARTICLES
	};

	ParticleSizeFilter(ImageSource::Ptr src, FilterType type, int erosions);
	virtual ImgPtr GetImage();

private:
	FilterType a_FilterType;
	int a_Erosions;
};
