#pragma once
#include<opencv2\opencv.hpp>
#include<opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2\nonfree\nonfree.hpp>
#include<opencv2\legacy\legacy.hpp>
#include<vector>

#include "img_registration.h"


class RegistrationSift : public ImgRegistration{
public:
	RegistrationSift() {};
	RegistrationSift(cv::Mat const &tmpl, cv::Mat const &test) :ImgRegistration(tmpl, test) {};
	RegistrationSift(std::string const &tmpl_file, std::string const &test_file) :ImgRegistration(tmpl_file, test_file) {};

	void DetectFeaturePoints(const cv::Mat& img, cv::Mat& dest, std::vector<cv::KeyPoint>& keypoints , cv::Mat& descriptors);
	void RegistrateTestImg();
	void ShowResultImg();
};
