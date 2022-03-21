#include "stdafx.h"
#include "calibrator.h"
#include <math.h> 

Calibrator::Calibrator()
{
}


Calibrator::~Calibrator()
{
}

void Calibrator::refine_eyes()
{	
	/*
	PA01 = cv::Point(265, 387);
	PA02 = cv::Point(371, 352);
	PA03 = cv::Point(392, 400);
	PA04 = cv::Point(302, 422);
	*/
	
		
	//std::cout << "PA01:\t" << PA01 << '\n';
	//std::cout << "PA02:\t" << PA02 << '\n';
	//std::cout << "PA03:\t" << PA03 << '\n';
	//std::cout << "PA04:\t" << PA04 << '\n';	

	PA02 = cv::Point2f(PA02.x - PA01.x, PA02.y - PA01.y);
	PA03 = cv::Point2f(PA03.x - PA01.x, PA03.y - PA01.y);
	PA04 = cv::Point2f(PA04.x - PA01.x, PA04.y - PA01.y);
	PA00 = PA01;
	PA01 = cv::Point2f(0, 0);
	
	const bool condition0 = PA02.y == 0 && PA04.y == 0;
	const bool condition1 = (PA02.x - PA01.x) / (PA02.y - PA01.y) == (PA04.x - PA03.x) / (PA04.y - PA03.y);
	const bool condition2 = (PA04.x - PA01.x) / (PA04.y - PA01.y) == (PA03.x - PA02.x) / (PA03.y - PA02.y);
		
	isCalibrated = !(condition0 || condition1 || condition2);

	//std::cout << "PA00:\t" << PA00 << '\n';
	//std::cout << "PA01:\t" << PA01 << '\n';
	//std::cout << "PA02:\t" << PA02 << '\n';
	//std::cout << "PA03:\t" << PA03 << '\n';
	//std::cout << "PA04:\t" << PA04 << '\n';
	

	t = -1 * atan2(PA02.y , PA02.x);
	//std::cout << "tan:\t" << t << '\n';

	PA02 = cv::Point2f(PA02.x * cos(t) - PA02.y * sin(t), PA02.x * sin(t) + PA02.y * cos(t));
	PA03 = cv::Point2f(PA03.x * cos(t) - PA03.y * sin(t), PA03.x * sin(t) + PA03.y * cos(t));
	PA04 = cv::Point2f(PA04.x * cos(t) - PA04.y * sin(t), PA04.x * sin(t) + PA04.y * cos(t));		

	monitorCoef = cv::Point2f(width / PA02.x, height / PA04.y);
	/*
	std::cout << "width:\t" << width << '\n';
	std::cout << "height:\t" << height << '\n';
	std::cout << "==============\n";
	std::cout << "Monitor Coeffs:\t" << monitorCoef << '\n';

	std::cout << "==============\n";
	std::cout << "tan:\t" << t << '\n';
	std::cout << "PA01 revert:\t" << PA01 << '\n';
	std::cout << "PA02 revert:\t" << PA02 << '\n';
	std::cout << "PA03 revert:\t" << PA03 << '\n';
	std::cout << "PA04 revert:\t" << PA04 << '\n';
	*/
	

	k1_4 = PA04.y / PA04.x;
	b1_4 = PA01.y - k1_4 * PA01.x;

	P1 = cv::Point2f(PA04.x, PA04.x * k1_4 + b1_4);
	P2 = cv::Point2f(0, b1_4);

	k2_3 = (PA03.y - PA02.y) / (PA03.x - PA02.x);
	b2_3 = PA03.y - k2_3 * PA03.x;

	P3 = cv::Point2f(PA03.x, PA03.x * k2_3 + b2_3);
	P4 = cv::Point2f(PA02.x, PA02.x * k2_3 + b2_3);

	
	//std::cout << "P1:\t" << P1 << '\n';
	//std::cout << "P2:\t" << P2 << '\n';
	//std::cout << "P3:\t" << P3 << '\n';
	//std::cout << "P4:\t" << P4 << '\n';	
	//std::cout << "=====================\n";
	
	
	float ch = -((P1.x * P2.y - P2.x * P1.y) * (P4.x - P3.x) - (P3.x * P4.y - P4.x * P3.y) * (P2.x - P1.x));
	//std::cout << ch << '\n';
	float z = ((P1.y - P2.y) * (P4.x - P3.x) - (P3.y - P4.y) * (P2.x - P1.x));
	//std::cout << z << '\n';
	tp1.x = -((P1.x * P2.y - P2.x * P1.y) * (P4.x - P3.x) - (P3.x * P4.y - P4.x * P3.y) * (P2.x - P1.x)) / ( (P1.y - P2.y) * (P4.x - P3.x) - (P3.y - P4.y) * (P2.x - P1.x) );

	tp1.y = ( (P3.y - P4.y) * -1 * tp1.x - (P3.x * P4.y - P4.x * P3.y) ) / (P4.x - P3.x);

	k3_4 = (PA04.y - PA03.y) / (PA04.x - PA03.x);
	b3_4 = PA04.y - k3_4 * PA04.x;

	tp2.x = -b3_4 / k3_4;
	tp2.y = 0;

	//std::cout << "k1_4:\t" << k1_4 << '\n';
	//std::cout << "k2_3:\t" << k2_3 << '\n';
	//std::cout << "b2_3:\t" << b2_3 << '\n';

	//std::cout << "tp1:\t" << tp1 << '\n';
	//std::cout << "tp2:\t" << tp2 << '\n';
		
	//std::cout << "width:\t" << width << '\n';
	//std::cout << "height:\t" << height << '\n';
	//std::cout << "monitorCoef:\t" << monitorCoef << '\n';
	
}

bool Calibrator::check_lines()
{
	return isCalibrated;
}

void Calibrator::update_eyes()
{	
	trackerPoints = eyes.vector;		
	PA01 = trackerPoints.at(0);
	PA02 = trackerPoints.at(1);
	PA03 = trackerPoints.at(2);
	PA04 = trackerPoints.at(3);
	eyeCenter = eyes.center;
}

void Calibrator::calculate_cursor()
{
	
	eyeCenterPrev = eyeCenter;
	const int length = 1024;
	cv::Point2f arr[length];
	for (int i = 0; i < length; ++i)
	{
		arr[i] = cv::Point2f(eyes.center.x - PA00.x, eyes.center.y - PA00.y);

		arr[i].x = arr[i].x * cos(t) - arr[i].y * sin(t);
		arr[i].y = arr[i].x * sin(t) + arr[i].y * cos(t);

		const bool condition0 = arr[i].x >= PA01.x && arr[i].y >= PA01.y;
		const bool condition1 = arr[i].x <= PA02.x && arr[i].y >= PA02.y;
		const bool condition2 = arr[i].x <= PA03.x && arr[i].y <= PA03.y;
		const bool condition3 = arr[i].x >= PA04.x && arr[i].y <= PA04.y;
		const bool result = condition0 || condition1 || condition2 || condition3;

		if (!result) {
			--i;
		}
		else
		{
			eyeCenter.x += arr[i].x;
			eyeCenter.y += arr[i].y;
		}
	}
	
	eyeCenter.x = eyeCenter.x / length;
	eyeCenter.y = eyeCenter.y / length;
	
	//eyeCenterPrev = eyeCenter;
	//std::cout << "eyeCenter on tracker:\t" << eyes.center << '\n';
	//std::cout << "=====================\n";
	cv::Point2f temp00 = cv::Point2f(eyes.center.x - PA00.x, eyes.center.y - PA00.y);	

	eyeCenter.x = temp00.x * cos(t) - temp00.y * sin(t);

	eyeCenter.y = temp00.x * sin(t) + temp00.y * cos(t);

	//std::cout << "eyeCenter on tracker reverse:\t" << eyeCenter << '\n';
	//std::cout << "=====================\n";

	const bool condition0 = eyeCenter.x < 0 && eyeCenter.y < 0;
	const bool condition1 = eyeCenter.x > PA02.x && eyeCenter.y > PA02.y;
	const bool condition2 = eyeCenter.x < 0 && eyeCenter.y > PA04.y;
	const bool condition3 = eyeCenter.x > PA02.x && eyeCenter.y > PA04.y;
	
	const bool result = condition0 || condition1 || condition2 || condition3;
	if (!result)
	{
		cv::Point2f tp;
		tp.x = -((P1.x * P2.y - P2.x * P1.y) * (eyeCenter.x - tp2.x) - (tp2.x * eyeCenter.y - eyeCenter.x * tp2.y) * (P2.x - P1.x))
			/ ((P1.y - P2.y) * (eyeCenter.x - tp2.x) - (tp2.y - eyeCenter.y) * (P2.x - P1.x));
		tp.y = ((tp2.y - eyeCenter.y) * -1 * tp.x - (tp2.x * eyeCenter.y - eyeCenter.x * tp2.y)) / (eyeCenter.x - tp2.x);

		float k = (tp1.y - eyeCenter.y) / (tp1.x - eyeCenter.x);
		float b = eyeCenter.y - k * eyeCenter.x;

		//std::cout << "K:\t" << k << '\n';
		//std::cout << "B:\t" << b << '\n';
		//std::cout << "=====================\n";
		eyeCenter.x = -b / k * monitorCoef.x;
		//std::cout << "eyeCenter.x\t" <<-b / k << '\n';
		//std::cout << "eyeCenter.y\t" << tp.y << '\n';
		//std::cout << "=====================\n";
		eyeCenter.y = tp.y * monitorCoef.y;	

		eyeCenter.x = (int)(eyeCenter.x - 0.0F * tp.x);
		eyeCenter.y = (int)(eyeCenter.y - 0.0F * tp.y);

		int delta = 2;

		if (eyeCenter.x - eyeCenterPrev.x > delta)
		{
			eyeCenter.x += delta;
		}
		if (eyeCenter.x - eyeCenterPrev.x < -delta)
		{
			eyeCenter.x -= delta;
		}

		if (eyeCenter.y - eyeCenterPrev.y > delta)
		{
			eyeCenter.y += delta;
		}
		if (eyeCenter.y - eyeCenterPrev.y < -delta)
		{
			eyeCenter.y -= delta;
		}
	}
	else
	{
		eyeCenter = eyeCenterPrev;

	}
	
	//std::cout << "eyeCenter on Monitor\t" << eyeCenter <<  '\n';

}

void Calibrator::debug()
{
	eyeCenterPrev = cv::Point(0, 0);	
	for (int y = 0; y < PA04.y; ++y)
	{
		for (int x = 0; x < PA02.x; ++x)
		{			
			//std::cout << "eyeCenter on tracker:\t" << eyes.center << '\n';
			//std::cout << "=====================\n";		
			
			eyeCenter = cv::Point2f(x, y);
			//eyeCenter.x = temp00.x * cos(t) - temp00.y * sin(t);

			//eyeCenter.y = temp00.x * sin(t) + temp00.y * cos(t);

			//std::cout << "eyeCenter on tracker reverse:\t" << eyeCenter << '\n';
			//std::cout << "=====================\n";

			const bool condition0 = eyeCenter.x < 0 && eyeCenter.y < 0;
			const bool condition1 = eyeCenter.x > PA02.x && eyeCenter.y > PA02.y;
			const bool condition2 = eyeCenter.x < 0 && eyeCenter.y > PA04.y;
			const bool condition3 = eyeCenter.x > PA02.x && eyeCenter.y > PA04.y;

			const bool result = condition0 || condition1 || condition2 || condition3;
			if (!result)
			{
				cv::Point2f tp;
				tp.x = -((P1.x * P2.y - P2.x * P1.y) * (eyeCenter.x - tp2.x) - (tp2.x * eyeCenter.y - eyeCenter.x * tp2.y) * (P2.x - P1.x))
					/ ((P1.y - P2.y) * (eyeCenter.x - tp2.x) - (tp2.y - eyeCenter.y) * (P2.x - P1.x));
				tp.y = ((tp2.y - eyeCenter.y) * -1 * tp.x - (tp2.x * eyeCenter.y - eyeCenter.x * tp2.y)) / (eyeCenter.x - tp2.x);

				float k = (tp1.y - eyeCenter.y) / (tp1.x - eyeCenter.x);
				float b = eyeCenter.y - k * eyeCenter.x;

				//std::cout << "K:\t" << k << '\n';
				//std::cout << "B:\t" << b << '\n';
				//std::cout << "=====================\n";
				eyeCenter.x = -b / k * monitorCoef.x;
				//std::cout << "eyeCenter.x\t" <<-b / k << '\n';
				//std::cout << "eyeCenter.y\t" << tp.y << '\n';
				//std::cout << "=====================\n";
				eyeCenter.y = tp.y * monitorCoef.y;

				eyeCenter.x = eyeCenter.x - 0.5F * tp.x;
				eyeCenter.y = eyeCenter.y - 0.5F * tp.y;
			}
			draw_lines();
			draw_points();
			move_cursor();			
			eyeCenterPrev = eyeCenter;
			std::cout << eyeCenter << '\n';
		}
	}
}
void Calibrator::move_cursor()
{		
	SetCursorPos(eyeCenter.x, eyeCenter.y);
}

void Calibrator::draw_points()
{	
	
	cv::circle(matrix.calibrationWindow, eyeCenter, 8, cv::Scalar(0, 0, 0), -1);

	cv::imshow("calibrationPoints", matrix.calibrationWindow);	
}

void Calibrator::draw_lines()
{
	cv::line(matrix.calibrationWindow, eyeCenterPrev, eyeCenter, cv::Scalar(0, 0, 255));
	cv::imshow("calibrationPoints", matrix.calibrationWindow);
}

cv::Point2f Calibrator::getEye()
{
	return eyeCenter;
}