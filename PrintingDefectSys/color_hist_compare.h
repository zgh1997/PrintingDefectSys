#pragma once

#include<opencv2\opencv.hpp>

#include "defect.h"
#include "img_pair_process.h"

class ColorHistCompare : public ImgPairProcess{
protected:
	// ��ɫȱ����Ϣ�б�
	std::vector<Defect> _color_defects;
	// ������ͼ��
	cv::Mat _result_img;
public:
	void Pretreat() {

	}

};