#pragma once

#include<opencv2\opencv.hpp>
#include "img_pair_process.h"

class ImgRegistration:public ImgPairProcess {
protected:
	// 处理结果图像(test image 对齐 template image后的差分图像)
	cv::Mat _result_img;
	// 特征点
	std::vector<cv::KeyPoint> keyPoint1;
	std::vector<cv::KeyPoint> keyPoint2;
public:
	ImgRegistration(cv::Mat const &tmpl, cv::Mat const &test) : ImgPairProcess(tmpl, test){}
	ImgRegistration(std::string const &tmpl_file, std::string const &test_file) : ImgPairProcess(tmpl_file, test_file) {}
	virtual void DetectFeaturePoints() = 0;
	virtual void ShowMatchedFeatures() = 0;
	virtual void RegistrateTestImg() = 0;
};