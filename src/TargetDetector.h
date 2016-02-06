#pragma once
#include <WPILib.h>

class TargetDetector {
public:
	TargetDetector(std::string ip);
	~TargetDetector();

	void SnapImage();

	void EnableDebugMode();
	void DisableDebugMode();
	bool GetDebugMode();

private:
	static void CheckIMAQError(int rval, std::string desc);
	void ImageProcessingTask();

	std::atomic_bool a_DebugMode;

	std::thread a_ImageProcessingTask;
	AxisCamera a_Camera;

	std::mutex a_ImageMutex;
	HSLImage a_Image;
};
