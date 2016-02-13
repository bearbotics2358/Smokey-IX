#pragma once
#include "ImageSource.h"
#include <Vision/AxisCamera.h>

class AxisImageSource: public ImageSource {
public:
	AxisImageSource(std::string ip);

	virtual bool IsNewImageAvailable();
	virtual ImgPtr GetImage();

private:
	AxisCamera a_Camera;
	ImgPtr a_CurrentImage;
};
