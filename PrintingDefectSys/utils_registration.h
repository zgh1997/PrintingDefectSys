#pragma once
#include<opencv2\core\core.hpp>
#include<opencv2\opencv.hpp>
#include<opencv2\highgui\highgui.hpp>

class UtilsRegistration {
public:
	void DrawMatch(const cv::Mat &src, const cv::Mat &obj, std::vector<cv::Point2f> &srcPoints, std::vector<cv::Point2f> &dstPoints);
	void FindInliers(std::vector<cv::KeyPoint> &qKeypoints, std::vector<cv::KeyPoint> &objKeypoints, std::vector<cv::DMatch> &matches, const cv::Mat &src, const cv::Mat &obj);
};