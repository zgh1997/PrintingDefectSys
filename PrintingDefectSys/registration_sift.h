#pragma once
#include<opencv2\opencv.hpp>

#include<opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2\nonfree\nonfree.hpp>
#include<opencv2\legacy\legacy.hpp>

#include "img_registration.h"
#include<vector>

class RegistrationSift : public ImgRegistration{
public:
	RegistrationSift(cv::Mat const &tmpl, cv::Mat const &test) :ImgRegistration(tmpl, test) {};
	RegistrationSift(std::string const &tmpl_file, std::string const &test_file) :ImgRegistration(tmpl_file, test_file) {};

	void DetectFeaturePoints();
	void ShowMatchedFeatures();
	void RegistrateTestImg();
};
