#include "stdafx.h"
#include "pupilFitterNew.h"
#include "globals.h"
#include "calibrator.h"
#include <fstream>

std::vector<cv::Point2f> pointVector;
auto timer = std::time(nullptr);
cv::VideoCapture camera(1);
PupilFitter pupilFitter;
Calibrator calibrator;
bool isCalibrated = false;
bool isPupilFind = false;
int pictureNumber = 0;

int keyEvents(int exit, int addCalibrationPoint, int startCalibration, int stopCalibration)
{	
	const int keyPressed = cv::waitKey(1);
	
	if (keyPressed == addCalibrationPoint)
	{	
		eyes.vector.push_back(eyes.center);

		for (auto &p : eyes.vector)
		{
			cv::circle(matrix.frame, p, 4, cv::Scalar(0, 0, 255), -1);			
		}		

		cv::imshow("points", matrix.frame);		
	}

	if (keyPressed == startCalibration)
	{
		
		isPupilFind = true;

		srand(time(NULL));
		pictureNumber = rand() % 15 + 1;
		matrix.calibrationWindow = cv::imread("pic\\" + std::to_string(pictureNumber) + ".jpg");
		//matrix.calibrationWindow = cv::imread("pic\\Тест.jpg");

		cv::imshow("calibrationPoints", matrix.calibrationWindow);
		timer = std::time(nullptr);

	}
	if (keyPressed == stopCalibration && !isCalibrated)
	{			
		calibrator.update_eyes();
		calibrator.refine_eyes();
		isCalibrated = calibrator.check_lines();			
		cv::destroyWindow("points");	
		cv::destroyWindow("frame");
	}	
	return keyPressed;
}
using namespace cv;

int main(int argc, char* argv[]) {
	/* Settings UI*/	
	int roi = 240;			
	int roiY = 100;	
	int lowThreshold = 10;	
	int highThreshold = 20;	
	int darkestPix1 = 10;	
	int darkestPix2 = 20;

	/*
	cv::namedWindow("settings", WINDOW_GUI_NORMAL);
	cv::createTrackbar("ROI", "settings", &roi, 240, nullptr, &roi);
	cv::createTrackbar("ROIY", "settings", &roiY, 240, nullptr, &roiY);
	cv::createTrackbar("lowThreshold", "settings", &lowThreshold, 40, nullptr, &lowThreshold);
	cv::createTrackbar("highThreshold", "settings", &highThreshold, 60, nullptr, &highThreshold);
	cv::createTrackbar("darkestPix1", "settings", &darkestPix1, 40, nullptr, &darkestPix1);
	cv::createTrackbar("darkestPix2", "settings", &darkestPix2, 50, nullptr, &darkestPix2);
	*/	
	
	pupilFitter.setDebug(false);

	if (argc > 1) 
	{		
		camera.open(std::atoi(argv[1]));
	}

	if (!camera.isOpened()) 
	{
		return -1;
	}
	
	for (auto &p: vectors.calibratedPoints)
	{
		cv::circle(matrix.calibrationWindow, p, 40, Scalar(0, 255, 0), -1);
	}
	
	cv::imshow("calibrationPoints", matrix.calibrationWindow);	

	for (; keyEvents(27, 32, 8, 13) != 27;)
	{		
		camera >> matrix.frame;				
		cv::flip(matrix.frame, matrix.frame, 1);	
		cv::cvtColor(matrix.frame, matrix.gray, CV_BGR2GRAY);		
		pupilFitter.pupilAreaFitRR(matrix.gray, eyes.ellipse, vectors.allPtsReturn, 10, roi, roi, lowThreshold, highThreshold, 240, darkestPix1, darkestPix2);
		
		if (isCalibrated)
		{
			if (!isPupilFind)
			{
				calibrator.calculate_cursor();				
				calibrator.draw_lines();
				calibrator.draw_points();
				calibrator.move_cursor();
			}
			else
			{
				calibrator.calculate_cursor();
				calibrator.move_cursor();
				pointVector.push_back(calibrator.getEye());	
				
				if (std::time(nullptr) - timer > matrix.time[pictureNumber])
				{
					break;
				}
			}
		}
		else
		{
			cv::line(matrix.frame, cv::Point(roi, 0), cv::Point(roi, matrix.frame.rows), cv::Scalar(0, 255, 0));
			cv::line(matrix.frame, cv::Point(matrix.frame.cols - roi, 0), cv::Point(matrix.frame.cols - roi, matrix.frame.rows), cv::Scalar(0, 255, 0));
			cv::ellipse(matrix.frame, eyes.ellipse, cv::Scalar(0, 200, 0), 2);
			cv::circle(matrix.frame, eyes.center, 4, cv::Scalar(0, 0, 255), -1);	
			cv::imshow("frame", matrix.frame);
		}	

		/*if (time(NULL) - timer > matrix.time[pictureNumber] && pictureNumber != 0) 
		{		
			break;
		}*/
		
	}
	cv::destroyWindow("calibrationPoints");
	std::ofstream savePoint("savePoints.txt");
	for (; keyEvents(27, 32, 8, 13) != 27;)
	{			
		for (cv::Point2f point : pointVector)
		{
			cv::circle(matrix.calibrationWindow, point, 10, cv::Scalar(66, 61, 254), -1);
			cv::imshow("calibrationPoints", matrix.calibrationWindow);
			
			savePoint << point.x << "," << point.y << '\n';
		}
		pointVector.clear();		
	}
	savePoint.close();
	camera.release();
	cv::destroyAllWindows();	
    return 0;
}