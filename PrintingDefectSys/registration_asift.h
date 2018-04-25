#pragma once

#include<opencv2\opencv.hpp>

#include<opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2\nonfree\nonfree.hpp>
#include<opencv2\legacy\legacy.hpp>
#include<vector>
#include<cmath>

#include "img_registration.h"

#define M_PI 3.14159265358979323846


class RegistrationASift : public ImgRegistration {
public:
	RegistrationASift(cv::Mat const &tmpl, cv::Mat const &test) :ImgRegistration(tmpl, test) {};
	RegistrationASift(std::string const &tmpl_file, std::string const &test_file) :ImgRegistration(tmpl_file, test_file) {};

	void DetectAndCompute(const cv::Mat& img, std::vector< cv::KeyPoint >& keypoints, cv::Mat& descriptors);
	void RegistrateTestImg();
	void ShowResultImg();
	void AffineSkew(double tilt, double phi, cv::Mat& img, cv::Mat& mask, cv::Mat& Ai);
};
