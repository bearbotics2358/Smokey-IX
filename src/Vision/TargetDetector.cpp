#include "TargetDetector.h"
#include "SimpleFilters.h"

using namespace std;

TargetDetector::TargetDetector(string ip):
		a_DebugMode(true), a_Processing(false),
		a_ImageProcessingTask(&TargetDetector::ImageProcessingTask, this),
		a_Camera(ip) {
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

void TargetDetector::SaveImage(string name, Image *img) {
	if (a_DebugMode) {
		string filename("/home/lvuser/" + name + ".jpg");
	    CheckIMAQError(
	        imaqWriteJPEGFile(img, filename.c_str(), 1000, NULL),
	        "imaqWriteJPEGFile");
	}
}

void TargetDetector::SnapImage() {
	lock_guard<mutex> guard(a_ImageMutex);
	a_Camera.GetImage(&a_Image);
}

void TargetDetector::EnableDebugMode() {
	a_DebugMode = true;
}

void TargetDetector::DisableDebugMode() {
	a_DebugMode = false;
}

bool TargetDetector::GetDebugMode() {
	return a_DebugMode;
}

void TargetDetector::StartProcessing() {
	a_Processing = true;
}

void TargetDetector::StopProcessing() {
	a_Processing = false;
}

bool TargetDetector::IsProcessing() {
	return a_Processing;
}

ImageFilter::Ptr TargetDetector::AppendProcessingChain(ImageSource::Ptr src) {
	ImageFilter::Ptr clusterThreshFilter(new ClusterThresholdFilter(src));
	ImageFilter::Ptr smallParticleFilter(new ParticleSizeFilter(
			clusterThreshFilter, ParticleSizeFilter::KEEP_LARGE_PARTICLES, 3));
	return smallParticleFilter;
}

void TargetDetector::ImageProcessingTask() {
	// Kernel for advanced morphology operation (remove small objects)
	int kernel[9] = {1, 1, 1, 1, 1, 1, 1, 1, 1};

	// Advanced morphology parameters (for removing small objects)
	StructuringElement structElem = {
			3, 3, FALSE, kernel
	};

	// Lookup table for creating binary images
	short lookupTable[256];
	lookupTable[0] = 0;
	for (int i = 1; i < 256; i++) {
		lookupTable[i] = 1;
	}

	// Target template image
	Image *targetTemplate = imaqCreateImage(IMAQ_IMAGE_U8, 0);
	CheckIMAQError(
			imaqReadFile(targetTemplate, "/home/lvuser/target_template.png", nullptr, nullptr),
			"imaqReadFile(targetTemplate)");

	// Normalize the target template
	CheckIMAQError(
			imaqLookup(targetTemplate, targetTemplate, lookupTable, nullptr),
			"imaqLookup");

	// Holds the results of the shape match
	ShapeReport* shapeReport = nullptr;
	int targetMatchesFound;

	// Images will be copied into this image as they arrive from the camera
	Image *curImage = imaqCreateImage(IMAQ_IMAGE_HSL, 0);

	// Holds the result of applying a threshold to the current image
	Image *postThreshold = imaqCreateImage(IMAQ_IMAGE_U8, 0);

	while (true) {
		if (!a_Processing) {
			// So we don't peg the CPU
			this_thread::sleep_for(chrono::milliseconds(10));
			continue;
		}

		// Copy the current frame to this thread
		{
			lock_guard<mutex> guard(a_ImageMutex);
			Image *image = a_Image.GetImaqImage();

			int width, height;
			CheckIMAQError(
					imaqGetImageSize(image, &width, &height),
					"imaqGetImageSize");

			CheckIMAQError(
					imaqDuplicate(curImage, image),
					"imaqDuplicate");
		}

		// Extract luminance plane
		Image *luminancePlane = imaqCreateImage(IMAQ_IMAGE_U8, 0);
		CheckIMAQError(
				imaqExtractColorPlanes(curImage, IMAQ_HSL, nullptr, nullptr, luminancePlane),
				"imaqExtractColorPlanes");
		SaveImage("01-extract-luminance", luminancePlane);

		// Auto thresholding (find bright objects)
		ThresholdData *threshData = imaqAutoThreshold2(
				curImage, luminancePlane, 2, IMAQ_THRESH_CLUSTERING, nullptr);
		if (threshData == nullptr) {
			CheckIMAQError(0, "imaqAutoThreshold2");
		}
		SaveImage("02-threshold", curImage);

		// Filters particles based on their size
		CheckIMAQError(
				imaqSizeFilter(curImage, curImage, TRUE, 3, IMAQ_KEEP_LARGE, &structElem),
				"imaqSizeFilter");
		SaveImage("03-remove-small-particles", curImage);

		// Fill holes
		CheckIMAQError(
				imaqFillHoles(curImage, curImage, TRUE),
				"imaqFillHoles");
		SaveImage("04-fill-holes", curImage);

		shapeReport = imaqMatchShape(curImage, curImage, targetTemplate,
				TRUE, 1, 0.5, &targetMatchesFound);
		if (shapeReport == nullptr) {
			CheckIMAQError(0, "imaqMatchShape");
		}

		if (a_DebugMode) {
			for (int i = 0; i < targetMatchesFound; i++) {
				ShapeReport shape = shapeReport[i];
				if (shape.score >= 500.0) {
					cout	<< "# Match " << i << endl
							<< "- score: " << shape.score << endl
							<< "- center: (" << shape.centroid.x << ", " << shape.centroid.y << ")" << endl
							<< "- size: (" << shape.size << endl;
				}
			}
		}

		imaqDispose(shapeReport);
		imaqDispose(luminancePlane);
	}

	imaqDispose(curImage);
	imaqDispose(postThreshold);
	imaqDispose(targetTemplate);
}
