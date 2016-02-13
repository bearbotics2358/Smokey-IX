#pragma once
#include "ImageSource.h"
#include <Vision/AxisCamera.h>

class AxisImageSource: public ImageSource {
public:
	AxisImageSource(std::string ip);

	virtual bool IsNewImageAvailable();
	virtual ImgRef GetImage();

private:
	AxisCamera a_Camera;
	ImgRef a_CurrentImage;
};
