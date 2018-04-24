#pragma once

#include<opencv2\opencv.hpp>

#include "defect.h"
#include "img_pair_process.h"

class ColorHistCompare : public ImgPairProcess{
protected:
	// 颜色缺陷信息列表
	std::vector<Defect> _color_defects;
	// 处理结果图像
	cv::Mat _result_img;
public:
	void Pretreat() {

	}

};