#include "img_pair_process.h"

ImgPairProcess::ImgPairProcess(cv::Mat const &tmpl, cv::Mat const &test)
{
	this->_tmpl_src = tmpl;
	this->_test_src = test;
}

ImgPairProcess::ImgPairProcess(std::string const &tmpl_file, std::string const &test_file)
{
	this->_tmpl_src = cv::imread(tmpl_file);
	this->_test_src = cv::imread(test_file);
}