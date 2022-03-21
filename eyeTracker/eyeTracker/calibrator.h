#pragma once
#include "globals.h"
#include <opencv2\opencv.hpp>
class Calibrator
{		
private:
	int& width = screen.width;
	int& height = screen.height;	
	float t = 0;	
	float k1_4, k2_3, k3_4 = 0;
	float b1_4, b2_3, b3_4 = 0;
	bool isCalibrated = false;
	std::vector<cv::Point2f> trackerPoints{ cv::Point(0, 0), cv::Point(0, 0), cv::Point(0, 0), cv::Point(0, 0)};	
	

	cv::Point2f PA00;
	cv::Point2f PA01;
	cv::Point2f PA02;
	cv::Point2f PA03;
	cv::Point2f PA04;

	cv::Point2f P1;
	cv::Point2f P2;
	cv::Point2f P3;
	cv::Point2f P4;

	cv::Point2f eyeCenter, eyeCenterPrev;
	cv::Point2f tp1, tp2, monitorCoef, cursor;

public:		
	Calibrator();
	~Calibrator();	
	void update_eyes();
	void refine_eyes();
	bool check_lines();
	void calculate_cursor();
	void move_cursor();	
	void draw_points();	
	void draw_lines();	
	void debug();
	cv::Point2f getEye();
};

