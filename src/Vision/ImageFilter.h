#pragma once
#include "ImageSource.h"

class ImageFilter: public ImageSource {
public:
	typedef std::shared_ptr<ImageFilter> Ptr;

	ImageFilter(ImageSource::Ptr source);

	virtual bool IsNewImageAvailable() { return true; }
	virtual ImgPtr GetImage() = 0;

protected:
	ImageSource::Ptr a_Source;
};
