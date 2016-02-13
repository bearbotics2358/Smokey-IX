#include "StillImageSource.h"
#include <Vision/VisionAPI.h>

using namespace std;

StillImageSource::StillImageSource(string filename) {
	Image *image = imaqCreateImage(IMAQ_IMAGE_RGB, DEFAULT_BORDER_SIZE);
	imaqReadFile(image, filename.c_str(), nullptr, nullptr);
	a_Image.reset(image, imaqDispose);
}

bool StillImageSource::IsNewImageAvailable() {
	return true;
}

ImgRef StillImageSource::GetImage() {
	return a_Image;
}
