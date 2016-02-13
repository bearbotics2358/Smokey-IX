#pragma once
#include <memory>
#include <nivision.h>

typedef std::shared_ptr<Image> ImgPtr;

class ImageSource {
public:
	typedef std::shared_ptr<ImageSource> Ptr;

	virtual bool IsNewImageAvailable() = 0;
	virtual ImgPtr GetImage() = 0;
	virtual ~ImageSource() = default;
};
