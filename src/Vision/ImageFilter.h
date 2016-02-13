#pragma once
#include "ImageSource.h"

class ImageFilter: public ImageSource {
public:
	typedef std::shared_ptr<ImageSource> ImgSrcRef;

	ImageFilter(std::shared_ptr<ImageSource> source);

	virtual ImgRef GetImage() = 0;

protected:
	std::shared_ptr<ImageSource> a_Source;
};
