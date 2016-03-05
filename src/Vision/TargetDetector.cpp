#include "TargetDetector.h"
#include "SimpleFilters.h"

using namespace std;

typedef unique_ptr<Image, decltype(&imaqDispose)> UniqueImgPtr;
typedef unique_ptr<ShapeReport, decltype(&imaqDispose)> ShapeReportPtr;

TargetDetector::TargetDetector(string ip):
		a_DebugMode(true), a_Processing(false),
		a_ImageCaptureTask(&TargetDetector::ImageCaptureTask, this),
		a_ImageProcessingTask(&TargetDetector::ImageProcessingTask, this),
		a_Camera(ip) {
	a_Camera.WriteResolution(AxisCamera::kResolution_640x480);
	a_LightRing.SetColor(0, 0, 0, 255);
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
		string errorDesc = "imaqWriteJPEGFile(";
		errorDesc.append(name);
		errorDesc.append(")");
	    CheckIMAQError(
	        imaqWriteJPEGFile(img, filename.c_str(), 1000, NULL),
	        errorDesc);
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

void TargetDetector::ImageCaptureTask() {
	while (true) {
		if (a_Camera.IsFreshImage()) {
			SnapImage();
		}
	}
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
	UniqueImgPtr targetTemplate(imaqCreateImage(IMAQ_IMAGE_U8, 7), imaqDispose);

	try {
		CheckIMAQError(
				imaqReadFile(targetTemplate.get(), "/home/lvuser/target_template.png", nullptr, nullptr),
				"imaqReadFile(targetTemplate)");

		// Normalize the target template
		CheckIMAQError(
				imaqLookup(targetTemplate.get(), targetTemplate.get(), lookupTable, nullptr),
				"imaqLookup");
	} catch (exception &ex) {
		cout << ex.what() << endl;
		return;
	}

	// Holds the results of the shape match
	int targetMatchesFound;

	// Images will be copied into this image as they arrive from the camera
	UniqueImgPtr curImage(imaqCreateImage(IMAQ_IMAGE_HSL, 0), imaqDispose);
	UniqueImgPtr curMonoImage(imaqCreateImage(IMAQ_IMAGE_U8, 7), imaqDispose);

	// Used to display match results
	UniqueImgPtr debugImage(imaqCreateImage(IMAQ_IMAGE_U8, 7), imaqDispose);

	// Used for benchmarking
	uint64_t start, end;

	while (true) {
		try {
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
						imaqDuplicate(curImage.get(), image),
						"imaqDuplicate");
			}

			// Start the clock
			start = GetFPGATime();

			/*
			// Extract luminance plane
			CheckIMAQError(
					imaqExtractColorPlanes(curImage.get(), IMAQ_HSL, nullptr, nullptr, curMonoImage.get()),
					"imaqExtractColorPlanes");
			SaveImage("01-extract-luminance", curMonoImage.get());

			// Auto thresholding (find bright objects)
			ThresholdData *threshData = imaqAutoThreshold2(
					curMonoImage.get(), curMonoImage.get(), 2, IMAQ_THRESH_CLUSTERING, nullptr);
			if (threshData == nullptr) {
				CheckIMAQError(0, "imaqAutoThreshold2");
			}
			SaveImage("02-threshold", curMonoImage.get());
			*/

			// Extract hue plane
			CheckIMAQError(
					imaqExtractColorPlanes(curImage.get(), IMAQ_HSL, nullptr, nullptr, curMonoImage.get()),
					"imaqExtractColorPlanes");
			SaveImage("01-extract-hue", curMonoImage.get());
			
			if (a_DebugMode) {
				CheckIMAQError(
						imaqDuplicate(debugImage.get(), curMonoImage.get()),
						"imaqDuplicate");
			}

			// Threshold (extract blue content)
			CheckIMAQError(
					imaqThreshold(curMonoImage.get(), curMonoImage.get(), 220, 260, 1, 255),
					"imaqThreshold");
			SaveImage("02-threshold", curMonoImage.get());

			// Normalize to a binary image
			CheckIMAQError(
					imaqLookup(curMonoImage.get(), curMonoImage.get(), lookupTable, nullptr),
					"imaqLookup");
			SaveImage("03-normalize", curMonoImage.get());

			// Filters particles based on their size
			CheckIMAQError(
					imaqSizeFilter(curMonoImage.get(), curMonoImage.get(), TRUE, 3, IMAQ_KEEP_LARGE, &structElem),
					"imaqSizeFilter");
			SaveImage("04-remove-small-particles", curMonoImage.get());

			// Fill holes
			CheckIMAQError(
					imaqFillHoles(curMonoImage.get(), curMonoImage.get(), TRUE),
					"imaqFillHoles");
			SaveImage("05-fill-holes", curMonoImage.get());

			ShapeReportPtr shapeReport(
					imaqMatchShape(curMonoImage.get(), curMonoImage.get(), targetTemplate.get(),
							TRUE, 1, 0.5, &targetMatchesFound),
					imaqDispose);
			if (!shapeReport) {
				CheckIMAQError(0, "imaqMatchShape");
			}

			// Stop the clock and display the processing time
			end = GetFPGATime();
			SmartDashboard::PutNumber("Image Processing Time", (end-start));

			if (a_DebugMode) {
				for (int i = 0; i < targetMatchesFound; i++) {
					ShapeReport shape = shapeReport.get()[i];
					if (shape.score >= 500.0) {
						cout	<< "# Match " << i << endl
								<< "- score: " << shape.score << endl
								<< "- center: (" << shape.centroid.x << ", " << shape.centroid.y << ")" << endl
								<< "- size: " << shape.size << endl;

						// Draw rectangle on debug image
						imaqDrawShapeOnImage(debugImage.get(), debugImage.get(), shape.coordinates,
								IMAQ_DRAW_VALUE, IMAQ_SHAPE_RECT, 255.0);
					}
				}
				SaveImage("debug-output", debugImage.get());
			}
		} catch (runtime_error &ex) {
			cout << ex.what() << endl;
		}
	}
}
