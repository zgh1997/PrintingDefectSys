#pragma once

#include<opencv2\opencv.hpp>
#include "img_pair_process.h"

class ImgRegistration:public ImgPairProcess {
protected:
	// ������ͼ��(test image ���� template image��Ĳ��ͼ��)
	cv::Mat _result_img;
	// ������
	std::vector<cv::KeyPoint> keyPoint1;
	std::vector<cv::KeyPoint> keyPoint2;
public:
	ImgRegistration(cv::Mat const &tmpl, cv::Mat const &test) : ImgPairProcess(tmpl, test){}
	ImgRegistration(std::string const &tmpl_file, std::string const &test_file) : ImgPairProcess(tmpl_file, test_file) {}
	virtual void DetectFeaturePoints() = 0;
	virtual void ShowMatchedFeatures() = 0;
	virtual void RegistrateTestImg() = 0;
};