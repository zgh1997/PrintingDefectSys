#pragma once

#include<opencv2\opencv.hpp>

class ImgRegistration {
protected:
	// 原始图像
	cv::Mat _tmpl_img;
	cv::Mat _test_img;
	// 预处理后图像
	cv::Mat _tmpl_pre;
	cv::Mat _test_pre;
	// 处理结果图像
	cv::Mat _result_img;
	// 特征点
	std::vector<cv::KeyPoint> keyPoint1;
	std::vector<cv::KeyPoint> keyPoint2;
public:
	virtual void DetectFeaturePoints() = 0;
	virtual void ShowMatchedFeatures() = 0;
	virtual void RegistrateTestImg() = 0;
};