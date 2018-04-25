#pragma once

#include<opencv2\opencv.hpp>

class ImgPairProcess {
protected:
	cv::Mat _tmpl_src;
	cv::Mat _test_src;
public:
	cv::Mat _tmpl_dest;
	cv::Mat _test_dest;

	ImgPairProcess() {};
	ImgPairProcess(cv::Mat const &tmpl, cv::Mat const &test);
	ImgPairProcess(std::string const &tmpl_file, std::string const &test_file);
};
