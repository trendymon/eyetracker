#ifndef IRIS_GEOMETRYFIT_ELLIPSE_H
#define IRIS_GEOMETRYFIT_ELLIPSE_H

#include <opencv2\opencv.hpp>

cv::RotatedRect fit_ellipse(const std::vector<cv::Point2f> &edgePoints, cv::Mat_<float> mPupilSobelX, cv::Mat_<float> mPupilSobelY, std::vector<cv::Point2f> &bestInliers);

#endif // IRIS_GEOMETRYFIT_ELLIPSE_H