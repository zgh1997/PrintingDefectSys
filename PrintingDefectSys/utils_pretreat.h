#pragma once

#include<opencv2\opencv.hpp>
#include<opencv2\highgui\highgui.hpp>

class UtilsPretreat {
public:
	//void UtilsPretreat::Threshold_Demo(int, void*);
	void Threshold(const cv::Mat &src, cv::Mat &dest);
	//void SegmentationOTSU(const cv::Mat &src, cv::Mat &dest);
};