/* test_vid.cpp for FRC - Example 2-1 from OpenCV book

created 3/25/16 BD from test.cpp 
- from 3/23 version
- change to reading from video file and processing eah frame
updated 3/27/16
- LATER, add command line for following:
- don't print if > threshold
- add angle

info:
- to make OpenCV project, copy and edit CMakeLists.tst
$ cmake .
$ make

to run it:

bobd@HP-Mini:~/opencv/install_opencv/OpenCV/opencv-2.4.8/build$ ~/TechClub/2016/vision/opencv/test_vid/test_vid ~/TechClub/2016/p5_auto.mjpg 


 */

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdio.h>
#include <iostream>
#include <string.h>

#define TEMPLATE_FILE "/home/bobd/TechClub/2016/template.jpg"

int process(cv::Mat img, cv::Mat &imgDraw)
{
	// cv::Mat img = cv::imread(argv[1], -1);
	// if(img.empty()) return -1;
	// cv::namedWindow("Input", cv::WINDOW_AUTOSIZE);
	// cv::imshow("Input", img);

	// first convert to HSV
	cv::Mat imgHSV;
	cv::cvtColor(img, imgHSV, CV_BGR2HSV);

	// threshold image
	cv::Mat imgThresh;
	
	// extract blue
	int HMin = 75; // 120 from robot code
	int HMax = 1300; // 170 from robot code
	int SMin = 180;
	int SMax = 255;
	int VMin = 150;
	int VMax = 255;

	// cv::threshold(imgHSV, imgThresh, );
	cv::inRange(imgHSV, cv::Scalar(HMin, SMin, VMin), cv::Scalar(HMax, SMax, VMax), imgThresh);
	// cv::namedWindow("Threshold", cv::WINDOW_AUTOSIZE);
	// cv::imshow("Threshold", imgThresh);

	// erode and dilate
	cv::Mat imgOpen;
	cv::morphologyEx(imgThresh, imgOpen, CV_MOP_OPEN, cv::Mat());
	// cv::namedWindow("Open", cv::WINDOW_AUTOSIZE);
	// cv::imshow("Open", imgOpen);

	// find countours
	// cv::Mat imgDraw(img);
	imgDraw = img;
	cv::Mat imgCont(imgOpen);
	std::vector< std::vector< cv::Point> > contours;
	cv::findContours(imgCont, contours, cv::noArray(), cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE);
	imgCont = cv::Scalar::all(0);
	cv::drawContours(imgDraw, contours, -1, cv::Scalar::all(255));

	// ok, now lets get ready to match template.jpg
	cv::Mat imgTemplate = cv::imread(TEMPLATE_FILE, -1);
	if(imgTemplate.empty()) return -1;
	cv::cvtColor(imgTemplate, imgTemplate, CV_BGR2HSV);
	cv::inRange(imgTemplate, cv::Scalar(HMin, SMin, VMin), cv::Scalar(HMax, SMax, VMax), imgTemplate);
	cv::morphologyEx(imgTemplate, imgTemplate, CV_MOP_OPEN, cv::Mat());
	std::vector< std::vector< cv::Point> > contoursTemplate;
	cv::findContours(imgTemplate, contoursTemplate, cv::noArray(), cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE);
	// cv::namedWindow("Template", cv::WINDOW_AUTOSIZE);
	// cv::imshow("Template", imgTemplate);

	printf("Found %d contours\n", contours.size());
	// find bounding rectangle
	for(int i = 0; i < contours.size(); i++) {
		/*
		// bounding rectange
		cv::Rect rect1 = cv::boundingRect(contours[i]);
		cv::rectangle(imgDraw, rect1, cv::Scalar(0, 255, 0));
		*/
		// minimum area (rotated) rectangle
		cv::RotatedRect rect1 = cv::minAreaRect(contours[i]);

		cv::Point2f vertices[4];
		rect1.points(vertices);
		for (int j = 0; j < 4; j++) {
			cv::line(imgDraw, vertices[j], vertices[(j+1)%4], cv::Scalar(0,255,0));
		}

		// compute Moments so we can find center of each contour (for printing for now)
		cv::Moments mom1 = cv::moments(contours[i], true);
		cv::Point center(mom1.m10/mom1.m00, mom1.m01/mom1.m00);
		cv::Point ptText(center);
		// printf("m00: %lf m10: %lf m01: %lf x: %lf, y: %lf\n", mom1.m00, mom1.m10, mom1.m01,
		//			 mom1.m10/mom1.m00, mom1.m01/mom1.m00);

		// check match against template
		double match = cv::matchShapes(contoursTemplate[0], contours[i], CV_CONTOURS_MATCH_I3, 0);

		int fontFace = cv::FONT_HERSHEY_SIMPLEX;
		double fontScale = 0.5;
		char s1[255];
		// sprintf(s1, "#%d %0.2lf", i, match);
		if((match <= 5.0)) {
			sprintf(s1, "#%d %0.2lf", i, match);
			cv::putText(imgDraw, s1, ptText, fontFace, fontScale, cv::Scalar(0, 0, 255), 1); 

			sprintf(s1, "x: %d", center.x);
			ptText.y += 15;
			cv::putText(imgDraw, s1, ptText, fontFace, fontScale, cv::Scalar(0, 0, 255), 1); 

			sprintf(s1, "y: %d", center.y);
			ptText.y += 15;
			cv::putText(imgDraw, s1, ptText, fontFace, fontScale, cv::Scalar(0, 0, 255), 1); 
		} else {
			// cv::putText(imgDraw, s1, center, fontFace, fontScale, cv::Scalar(255, 255, 255), 2);
		}
	}
	// cv::namedWindow("Contours", cv::WINDOW_AUTOSIZE);
	// cv::imshow("Contours", imgDraw);

	// cv::waitKey(0);
	// cv::destroyAllWindows();

	return 0;
}

int main(int argc, char *argv[])
{
	cv::namedWindow("Contours", cv::WINDOW_AUTOSIZE);
	cv::VideoCapture cap;
	cap.open(std::string(argv[1]));
	cv::Mat frame;
	cv::Mat imgDraw;;
	while(1) {
		cap >> frame;
		if(!frame.data) break; // ran out of film
		process(frame, imgDraw);
		cv::imshow("Contours", imgDraw);
		if(cv::waitKey(33) >= 0) break;
	}
	return 0;
}

