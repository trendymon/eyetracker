#pragma once
#include <opencv2\opencv.hpp>

struct scr {
	int width = GetSystemMetrics(SM_CXSCREEN);
	int height = GetSystemMetrics(SM_CYSCREEN);
};
extern scr screen;
struct mat {
	cv::Mat frame;
	cv::Mat gray;
	cv::Mat calibrationWindow = cv::imread("pic\\0.jpg");
	int time[16] = { 25, 35, 25, 20, 25, 25, 20, 25, 25, 25, 30, 25, 35, 20, 25, 25 };
	//1 - 25
	//2 - 35
	//3 - 25
	//4 - 20
	//5 - 25
	//6 - 25
	//7 - 20
	//8 - 25
	//9 - 25
	//10 - 25
	//11 - 30
	//12 - 25
	//13 - 35
	//14 - 20
	//15 - 25
	//16 - 25
};
extern mat matrix;
struct vec {
	std::vector<cv::Point> allPtsReturn;
	std::vector<cv::Point2f> allPtsReturn2f;
	std::vector<cv::Point> calibratedPoints = { cv::Point(40, 40), cv::Point(screen.width - 40, 40),
		cv::Point(40, screen.height - 40),
		cv::Point(screen.width - 40, screen.height - 40) };
};
extern vec vectors;
struct eye {
	std::vector<cv::Point2f> vector;
	cv::RotatedRect ellipse;
	cv::Point2f &center = ellipse.center;
};
extern eye eyes;
