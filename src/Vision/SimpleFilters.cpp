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

ClusterThresholdFilter::ClusterThresholdFilter(ImageSource::Ptr src):
		ImageFilter(src) {
}

ImgPtr ClusterThresholdFilter::GetImage() {
	ImgPtr img = a_Source.get()->GetImage();
	imaqAutoThreshold2(
			img.get(), img.get(), 2, IMAQ_THRESH_CLUSTERING, nullptr);
	return img;
}

ParticleSizeFilter::ParticleSizeFilter(ImageSource::Ptr src, FilterType type, int erosions):
		ImageFilter(src), a_FilterType(type), a_Erosions(erosions) {
}

ImgPtr ParticleSizeFilter::GetImage() {
	int kernel[9] = {1, 1, 1, 1, 1, 1, 1, 1, 1};
	StructuringElement structElem = {
			3, 3, FALSE, kernel
	};

	ImgPtr img = a_Source.get()->GetImage();

	switch (a_FilterType) {
	case KEEP_SMALL_PARTICLES:
		imaqSizeFilter(img.get(), img.get(), TRUE, 3, IMAQ_KEEP_SMALL, &structElem);
		break;
	case KEEP_LARGE_PARTICLES:
		imaqSizeFilter(img.get(), img.get(), TRUE, 3, IMAQ_KEEP_LARGE, &structElem);
		break;
	}

	return img;
}
