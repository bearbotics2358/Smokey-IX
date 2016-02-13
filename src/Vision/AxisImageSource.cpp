#include "AxisImageSource.h"
#include <Vision/VisionAPI.h>

using namespace std;

AxisImageSource::AxisImageSource(string ip):
		a_Camera(ip) {
}

bool AxisImageSource::IsNewImageAvailable() {
	return a_Camera.IsFreshImage();
}

ImgPtr AxisImageSource::GetImage() {
	if (IsNewImageAvailable()) {
		Image *image = imaqCreateImage(IMAQ_IMAGE_RGB, DEFAULT_BORDER_SIZE);
		a_Camera.GetImage(image);
		a_CurrentImage.reset(image, imaqDispose);
	}
	return a_CurrentImage;
}
