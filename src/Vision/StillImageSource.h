#pragma once
#include "ImageSource.h"

class StillImageSource: public ImageSource {
public:
	StillImageSource(std::string filename);

	virtual bool IsNewImageAvailable();
	virtual ImgRef GetImage();

private:
	ImgRef a_Image;
};
