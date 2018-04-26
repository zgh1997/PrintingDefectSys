#include "img_registration.h"

void ImgRegistration::GetDifImg(cv::Mat const & tmpl, cv::Mat const & test, cv::Mat & dif, int d)
{
	int w = tmpl.cols, h = tmpl.rows;
	int rgb[3];
	dif = cv::Mat(h, w, CV_8UC3, cv::Scalar(2, 2, 2));
	for (int i = 1; i < h-1; i++) {
		for (int j = 1; j < w-1; j++) {
			// 模板图面积为3*3的像素领域内，欧式距离最小点
			uint64 dis = UINT64_MAX, tmp = 0;
			for (int k = i - 1; k < i + 1; k++) {
				for (int l = j - 1; l < j + 1; l++) {
					for (int c = 0; c < d; c++) rgb[c] = abs(tmpl.at<cv::Vec3b>(k, l)[c] - test.at<cv::Vec3b>(i, j)[c]);
					for (int c = 0; c < d; c++) tmp += rgb[c]*rgb[c];
					if (tmp < dis) {
						dis = tmp;
						for (int c = 0; c < d; c++) dif.at<cv::Vec3b>(i,j)[c] = rgb[c];
					}
				}
			}
		}
	}
}
