#include "TargetDetector.h"

using namespace std;

TargetDetector::TargetDetector(string ip):
		a_ImageProcessingTask(&TargetDetector::ImageProcessingTask, this), a_Camera(ip) {

}

TargetDetector::~TargetDetector() {

}

void TargetDetector::CheckIMAQError(int rval, string desc) {
  if (rval == 0) {
    int error = imaqGetLastError();

    ostringstream errorDesc;
    errorDesc << error << " " << desc;

    throw runtime_error(errorDesc.str());
  }
}

void TargetDetector::SnapImage() {
	lock_guard<mutex> guard(a_ImageMutex);
	a_Camera.GetImage(&a_Image);
}

void TargetDetector::ImageProcessingTask() {
	int rval;
	Image *curImage = imaqCreateImage(IMAQ_IMAGE_HSL, 0);
	while (true) {
		// Copy the current frame to this thread
		{
			lock_guard<mutex> guard(a_ImageMutex);
			Image *image = a_Image.GetImaqImage();

			int width, height;
			rval = imaqGetImageSize(image, &width, &height);
			CheckIMAQError(rval, "imaqGetImageSize");

			// TODO: Is there a more generic function for copying one image
			// to another?
			rval = imaqCopyRect(curImage, image, {0, 0, width, height}, {0, 0});
			CheckIMAQError(rval, "imaqCopyRect");
		}

		Image *hPlane = imaqCreateImage(IMAQ_IMAGE_U8, 0);
		Image *sPlane = imaqCreateImage(IMAQ_IMAGE_U8, 0);
		Image *lPlane = imaqCreateImage(IMAQ_IMAGE_U8, 0);
		imaqExtractColorPlanes(curImage, IMAQ_HSL, hPlane, sPlane, lPlane);

		imaqDispose(hPlane);
		imaqDispose(sPlane);
		imaqDispose(lPlane);
	}
	imaqDispose(curImage);
}
