#pragma once
#include "ImageFilter.h"
#include "../LightRingController.h"
#include <WPILib.h>

class TargetDetector {
public:
	TargetDetector(std::string ip);
	~TargetDetector();

	void SnapImage();

	void EnableDebugMode();
	void DisableDebugMode();
	bool GetDebugMode();

	void StartProcessing();
	void StopProcessing();
	bool IsProcessing();

	bool CanSeeTarget();
	double GetDistanceToTarget();
	double GetAngleToTarget();

private:
	static constexpr float M1013_HFOV_DEG = 67.0;
	static constexpr float M1013_VFOV_DEG = 49.3;
	static constexpr float VISION_TARGET_H_IN = 12.0;
	static constexpr float M1013_IMG_W = 640.0;
	static constexpr float M1013_IMG_H = 480.0;
	// angle of camera
	static constexpr float CAM_VERT_ANGLE = 24.5;
	// step 1
	static constexpr float PROJ_IMG_HEIGHT = cos(CAM_VERT_ANGLE) * VISION_TARGET_H_IN;

	static constexpr float MIN_TARGET_SCORE = 250.0;

	static void CheckIMAQError(int rval, std::string desc);

	static void GetTargetError(ShapeReport &shape, double &distance, double &angle);

	void SaveImage(std::string path, Image *img);
	void ImageCaptureTask();
	void ImageProcessingTask();

	ImageFilter::Ptr AppendProcessingChain(ImageSource::Ptr src);

	std::atomic_bool a_DebugMode;
	std::atomic_bool a_Processing;

	LightRingController a_LightRing;

	std::thread a_ImageCaptureTask;
	std::thread a_ImageProcessingTask;
	AxisCamera a_Camera;

	std::mutex a_ImageMutex;
	HSLImage a_Image;

	// Current measurement results
	std::atomic_bool a_CanSeeTarget;
	std::atomic<double> a_DistanceToTarget;
	std::atomic<double> a_AngleToTarget;
};
