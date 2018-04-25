#pragma once

#include<opencv2\opencv.hpp>
#include "img_pair_process.h"

#define SHOW_TEST_IMAGE

class ImgRegistration:public ImgPairProcess {
protected:
	// ������ͼ��(test image ���� template image��Ĳ��ͼ��)
	cv::Mat _result_img;
	// ������
	std::vector<cv::KeyPoint> _tmpl_keypoints;
	std::vector<cv::KeyPoint> _test_keypoints;
public:
	ImgRegistration() {};
	ImgRegistration(cv::Mat const &tmpl, cv::Mat const &test) : ImgPairProcess(tmpl, test){}
	ImgRegistration(std::string const &tmpl_file, std::string const &test_file) : ImgPairProcess(tmpl_file, test_file) {}

	virtual void RegistrateTestImg() = 0;
};