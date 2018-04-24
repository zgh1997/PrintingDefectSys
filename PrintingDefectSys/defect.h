#pragma once

#include<opencv2\opencv.hpp>
enum DEFECTS_TYPE {COLOR_A,COLORB, SHAPE_A, SHAPE_B, TEXT_A, TEXT_B};

class Defect {
public:
	cv::Rect _defect_area;// Defect position and area
	DEFECTS_TYPE _defect_type;
};