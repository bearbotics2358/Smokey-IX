#pragma once
#include <memory>
#include <nivision.h>

typedef std::shared_ptr<Image> ImgRef;

class ImageSource {
public:
	virtual bool IsNewImageAvailable() = 0;
	virtual ImgRef GetImage() = 0;
	virtual ~ImageSource() = default;
};
