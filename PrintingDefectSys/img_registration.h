#pragma once

#include<opencv2\opencv.hpp>
#include "img_pair_process.h"

#define FEATURE_POINT_NUM 800
//#define SHOW_TEST_IMAGE

class ImgRegistration:public ImgPairProcess {
protected:
	// 特征点
	std::vector<cv::KeyPoint> _tmpl_keypoints;
	std::vector<cv::KeyPoint> _test_keypoints;
public:
	// 处理结果图像(test image 对齐 template image后的差分图像)
	cv::Mat _result_img;

	ImgRegistration() {};
	ImgRegistration(cv::Mat const &tmpl, cv::Mat const &test) : ImgPairProcess(tmpl, test){}
	ImgRegistration(std::string const &tmpl_file, std::string const &test_file) : ImgPairProcess(tmpl_file, test_file) {}

	virtual void RegistrateTestImg() = 0;
	void GetDifImg(cv::Mat const &tmpl, cv::Mat const &test, cv::Mat &dif, int d = 3);
};