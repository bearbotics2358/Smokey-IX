#pragma once
#include <WPILib.h>

typedef std::shared_ptr<Image> ImgRef;

class ImageSource {
public:
	virtual bool IsNewImageAvailable() = 0;
	virtual ImgRef GetImage() = 0;
	virtual ~ImageSource() = default;
};

class AxisImageSource: public ImageSource {
public:
	AxisImageSource(std::string ip);

	virtual bool IsNewImageAvailable();
	virtual ImgRef GetImage();

private:
	AxisCamera a_Camera;
	ImgRef a_CurrentImage;
};

class StillImageSource: public ImageSource {
public:
	StillImageSource(std::string filename);

	virtual bool IsNewImageAvailable();
	virtual ImgRef GetImage();

private:
	ImgRef a_Image;
};

class ImageFilter: public ImageSource {
public:
	typedef std::shared_ptr<ImageSource> ImgSrcRef;

	ImageFilter(std::shared_ptr<ImageSource> source);

	virtual ImgRef GetImage() = 0;

protected:
	std::shared_ptr<ImageSource> a_Source;
};

class ThresholdFilter: public ImageFilter {
public:
	ThresholdFilter(ImgSrcRef src, int threshold);

	virtual ImgRef GetImage();

protected:
	int a_Threshold;
};

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
	void ImageProcessingTask();

	std::atomic_bool a_DebugMode;
	std::atomic_bool a_Processing;

	std::thread a_ImageProcessingTask;
	AxisCamera a_Camera;

	std::mutex a_ImageMutex;
	HSLImage a_Image;
};
