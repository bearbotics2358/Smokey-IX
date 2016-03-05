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

private:
	static void CheckIMAQError(int rval, std::string desc);

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
};
