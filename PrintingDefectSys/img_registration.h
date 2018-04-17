#pragma once

#include<opencv2\opencv.hpp>

class ImgRegistration {
protected:
	// ԭʼͼ��
	cv::Mat _tmpl_img;
	cv::Mat _test_img;
	// Ԥ�����ͼ��
	cv::Mat _tmpl_pre;
	cv::Mat _test_pre;
	// ������ͼ��
	cv::Mat _result_img;
	// ������
	std::vector<cv::KeyPoint> keyPoint1;
	std::vector<cv::KeyPoint> keyPoint2;
public:
	virtual void DetectFeaturePoints() = 0;
	virtual void ShowMatchedFeatures() = 0;
	virtual void RegistrateTestImg() = 0;
};