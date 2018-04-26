#include "utils_pretreat.h"
/*
* Threshold
*/
// Threshold parameters

int threshold_value = 0;
int threshold_type = 3;;
int const max_value = 255;
int const max_type = 8;
int const max_BINARY_value = 255;
char* window_name = "Threshold";

char* trackbar_type = "Type: \n 0: Binary \n 1: Binary Inverted \n 2: Truncate \n 3: To Zero \n 4: To Zero Inverted";
char* trackbar_value = "Value";
cv::Mat src, src_gray, dst;

void Threshold_Demo(int, void*)
{
	/* 0: ��������ֵ
	1: ����������ֵ
	2: �ض���ֵ
	3: 0��ֵ
	4: ��0��ֵ
	*/
	cv::threshold(src_gray, dst, threshold_value, max_BINARY_value, CV_THRESH_OTSU);
	// TODO: Test image segmentation algorithms
	// OTSU




	//cv::erode(dst, dst, (3, 3), cv::Point(-1, -1), 3);
	cv::imshow(window_name, dst);
}

void UtilsPretreat::Threshold(const cv::Mat &src_img, cv::Mat &dest)
{
	src = src_img;
	cv::cvtColor(src, src_gray, CV_RGB2GRAY);
	cv::namedWindow(window_name, CV_WINDOW_AUTOSIZE);

	/// ������������������ֵ
	cv::createTrackbar(trackbar_type,
		window_name, &threshold_type,
		max_type, Threshold_Demo);

	cv::createTrackbar(trackbar_value,
		window_name, &threshold_value,
		max_value, Threshold_Demo);

	Threshold_Demo(0, 0);

	/// �ȴ��û������������ESC�����˳��ȴ����̡�
	while (true)
	{
		int c;
		c = cv::waitKey(20);
		if ((char)c == 27)
		{
			break;
		}
	}
	dest = dst;
}

