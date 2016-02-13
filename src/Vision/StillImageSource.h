#pragma once
#include "ImageSource.h"

class StillImageSource: public ImageSource {
public:
	StillImageSource(std::string filename);

	virtual bool IsNewImageAvailable();
	virtual ImgPtr GetImage();

private:
	ImgPtr a_Image;
};
