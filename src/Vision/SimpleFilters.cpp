#include "SimpleFilters.h"

using namespace std;

ThresholdFilter::ThresholdFilter(ImageSource::Ptr src, int threshold):
		ImageFilter(src), a_Threshold(threshold) {
}

ImgPtr ThresholdFilter::GetImage() {
	ImgPtr img = a_Source.get()->GetImage();
	imaqThreshold(img.get(), img.get(), 0, a_Threshold, FALSE, 0.0);
	return img;
}
