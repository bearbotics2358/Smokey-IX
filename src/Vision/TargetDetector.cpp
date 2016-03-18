#include "TargetDetector.h"
#include "SimpleFilters.h"

using namespace std;

typedef unique_ptr<Image, decltype(&imaqDispose)> UniqueImgPtr;
typedef unique_ptr<ShapeReport, decltype(&imaqDispose)> ShapeReportPtr;

TargetDetector::TargetDetector(string ip):
		a_DebugMode(false), a_Processing(false),
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

void TargetDetector::GetTargetError(ShapeReport &shape, double &distance, double &angle) {
	// Available constants:
	// M1013_VFOV_DEG     - vertical FOV of camera
	// M1013_IMG_H        - height of picture in pixels
	// VISION_TARGET_H_IN - height of vision target in inches

	// M1013_HFOV_DEG	  - horizontal FOV of camera
	// M1013_IMG_W 		  - width of pictures in pixels

	// using center of image as (0,0) // + up and right
	double imgVertSizePx;
	double imgVertPosPx;
	double imgVertPosIn;
	double projImgAngle;
	double projDistanceBase;
	double projDistanceBaseExtra;
	double distanceHyp;
	double distanceBase;
	double inPerPx;

	double imgHorizPosPx;


	// step 2
	imgVertSizePx = shape.coordinates.height;
	imgVertPosPx = (0.5 * M1013_IMG_H) - shape.centroid.y;
	inPerPx = PROJ_IMG_HEIGHT / imgVertSizePx;
	imgVertPosIn = inPerPx * imgVertPosPx;

	// step 3
	projImgAngle = imgVertPosPx / M1013_IMG_H  * M1013_VFOV_DEG;
	projDistanceBase = 1 / tan(projImgAngle) * imgVertPosIn;

	// step 4
	projDistanceBaseExtra = tan(CAM_VERT_ANGLE) * imgVertPosIn;
	distanceHyp = projDistanceBase - projDistanceBaseExtra;

	// step 5
	distanceBase = cos(CAM_VERT_ANGLE) * distanceHyp;

	// Old math
	// double distanceToTarget = cot((M1013_VFOV_DEG/M1013_IMG_H)*targetHeight)*VISION_TARGET_H_IN;

	// Old math 2.0
	/*
	double distanceToTarget	= (VISION_TARGET_H_IN*sin((180-(90-M1013_VFOV_DEG))-(M1013_VFOV_DEG/M1013_IMG_H*targetHeight)))
							   /sin(M1013_VFOV_DEG/M1013_IMG_H*targetHeight);
	*/

	// SmartDashboard::PutNumber("Distance form Target",distanceBase);

	distance = distanceBase;

	// horizontal angle adjustment
	imgHorizPosPx = shape.centroid.x - 450.0;
	angle = M1013_HFOV_DEG * imgHorizPosPx / M1013_IMG_W;
	// SmartDashboard::PutNumber("Angle to Adjust",angle);


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
	a_LightRing.SetColor(0, 0, 0, 255);
	a_Processing = true;
}

void TargetDetector::StopProcessing() {
	a_Processing = false;
}

bool TargetDetector::IsProcessing() {
	return a_Processing;
}

bool TargetDetector::CanSeeTarget() {
	return a_CanSeeTarget;
}

double TargetDetector::GetDistanceToTarget() {
	return a_DistanceToTarget;
}

double TargetDetector::GetAngleToTarget() {
	return a_AngleToTarget;
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

	// HSL ranges for the color threshold operation
	Range hueRange { 120,   170 }; // Extract blue
	Range satRange { 180,   255 };
	Range valRange { 150,   255 };

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
	UniqueImgPtr curImage(imaqCreateImage(IMAQ_IMAGE_HSL, 7), imaqDispose);
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

			/* Extract luminance and cluster threshold
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

			/* Extract hue plane and threshold
			// Extract hue plane
			CheckIMAQError(
					imaqExtractColorPlanes(curImage.get(), IMAQ_HSL, nullptr, nullptr, curMonoImage.get()),
					"imaqExtractColorPlanes");
			SaveImage("01-extract-hue", curMonoImage.get());

			// Threshold (extract blue content)
			CheckIMAQError(
					imaqThreshold(curMonoImage.get(), curMonoImage.get(), 220, 260, 1, 255),
					"imaqThreshold");
			SaveImage("02-threshold", curMonoImage.get());
			*/

			// Extract blueish pixels
			CheckIMAQError(
					imaqColorThreshold(curMonoImage.get(), curImage.get(), 1, IMAQ_HSV,
							&hueRange, &satRange, &valRange),
					"imaqColorThreshold");
			SaveImage("02-threshold", curMonoImage.get());
			
			if (a_DebugMode) {
				CheckIMAQError(
						imaqDuplicate(debugImage.get(), curMonoImage.get()),
						"imaqDuplicate");
			}

			int kernel[9] = {0,1,0,1,1,1,0,1,0};
			StructuringElement element;
			element.matrixCols = 3;
			element.matrixRows = 3;
			element.hexa = FALSE;
			element.kernel = kernel;
			CheckIMAQError(
					imaqMorphology(curMonoImage.get(), curMonoImage.get(), IMAQ_ERODE, &element),
					"imaqMorphology(erode)");
			CheckIMAQError(
					imaqMorphology(curMonoImage.get(), curMonoImage.get(), IMAQ_DILATE, &element),
					"imaqMorphology(dilate)");

			// Filters particles based on their size
			CheckIMAQError(
					imaqSizeFilter(curMonoImage.get(), curMonoImage.get(), TRUE, 3, IMAQ_KEEP_LARGE, &structElem),
					"imaqSizeFilter");
			SaveImage("04-remove-small-particles", curMonoImage.get());

			/* Fill holes
			CheckIMAQError(
					imaqFillHoles(curMonoImage.get(), curMonoImage.get(), TRUE),
					"imaqFillHoles");
			SaveImage("05-fill-holes", curMonoImage.get());
			*/

			ShapeReportPtr shapeReport(
					imaqMatchShape(curMonoImage.get(), curMonoImage.get(), targetTemplate.get(),
							TRUE, 1, 0.5, &targetMatchesFound),
					imaqDispose);
			if (!shapeReport) {
				CheckIMAQError(0, "imaqMatchShape");
			}

			// Stop the clock and display the processing time
			end = GetFPGATime();
			SmartDashboard::PutNumber("Image Processing Time", (end-start)/1000000.0);

			SmartDashboard::PutNumber("Matches Found", targetMatchesFound);

			int maxShapeScore = 0;
			int maxShapeIndex = 0;
			int numValidTargets = 0;
			for (int i = 0; i < targetMatchesFound; i++) {
				ShapeReport shape = shapeReport.get()[i];
				if (shape.score >= MIN_TARGET_SCORE) {
					numValidTargets += 1;

					if (shape.score > maxShapeScore) {
						maxShapeScore = shape.score;
						maxShapeIndex = i;
					}

					if (a_DebugMode) {
						cout	<< "# Match " << i << endl
								<< "- score: " << shape.score << endl
								<< "- center: (" << shape.centroid.x << ", " << shape.centroid.y << ")" << endl
								<< "- size: " << shape.size << endl;

						// Draw rectangle on debug image
						imaqDrawShapeOnImage(debugImage.get(), debugImage.get(), shape.coordinates,
								IMAQ_DRAW_VALUE, IMAQ_SHAPE_RECT, 255.0);
					}
				}
			}

			cout << "numValidTargets: " << numValidTargets << endl;

			if (numValidTargets > 0) {
				a_CanSeeTarget = true;

				double distance, angle;
				GetTargetError(shapeReport.get()[maxShapeIndex], distance, angle);
				a_DistanceToTarget = distance;
				a_AngleToTarget = angle;
				cout << "angle to target: " << angle << endl;
			} else {
				a_CanSeeTarget = false;
			}

			if (a_DebugMode) {
				SaveImage("debug-output", debugImage.get());
			}
		} catch (runtime_error &ex) {
			cout << ex.what() << endl;
		}
	}
}

void TargetDetector::TurnLRCBright()
{
	a_LightRing.SetColor(0,0,0,255);
}

void TargetDetector::TurnLRCDown()
{
	a_LightRing.SetColor(0,0,0,32);
}
