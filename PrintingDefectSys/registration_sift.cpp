#include "registration_sift.h"
#define FEATURE_POINT_NUM 200
//#define SHOW_TEST_IMAGE

/*******供给外部调用*******/
/**************************/
/*
 * 检测img高斯滤波后相应的关键点
*/
void RegistrationSift::DetectFeaturePoints(const cv::Mat& img, cv::Mat& dest, std::vector<cv::KeyPoint>& keypoints , cv::Mat& descriptors)
{
	// 预处理：高斯滤波,便于准确提取尺度不变特征
	GaussianBlur(img, dest, cv::Size(3, 3), 0.5);
	//提取特征点
	std::vector<cv::KeyPoint> points;
	cv::SiftFeatureDetector siftDetector(FEATURE_POINT_NUM);  //提取前FEATURE_POINT_NUM个特征点    
	siftDetector.detect(dest, points);

	cv::SiftDescriptorExtractor extractor;
	extractor.compute(img, points, descriptors);
	keypoints.insert(keypoints.end(), points.begin(), points.end());

}



/*******对类中图像进行系列操作*******/
/**************************/

/*
** 根据匹配点配准
*/
void RegistrationSift::RegistrateTestImg()
{
	//特征点描述，为下边的特征点匹配做准备  
	cv::SiftDescriptorExtractor siftDescriptor;
	cv::Mat imageDesc1, imageDesc2;
	_tmpl_keypoints.clear();
	_test_keypoints.clear();
	this->DetectFeaturePoints(this->_tmpl_src, this->_tmpl_dest, this->_tmpl_keypoints, imageDesc1);
	this->DetectFeaturePoints(this->_test_src, this->_test_dest, this->_test_keypoints, imageDesc2);

	//特征点匹配并显示匹配结果  
	cv::BruteForceMatcher<cv::L2<float>> matcher;
	std::vector<cv::DMatch> matchePoints;
	matcher.match(imageDesc1, imageDesc2, matchePoints, cv::Mat());

	/*
	** 计算投影映射矩阵，将测试图配准到模板
	*/

	//// 选取最符合的50个特征点,舍去其他点
	//std::nth_element(matchePoints.begin(), matchePoints.begin() + 49, matchePoints.end());
	////matchePoints.erase(matchePoints.begin() + 100, matchePoints.end());

	// 计算投影映射矩阵
	std::vector<cv::Point2f> imagePoints1, imagePoints2;
	for (int i = 0; i < FEATURE_POINT_NUM; i++) {
		imagePoints1.push_back(_tmpl_keypoints[matchePoints[i].queryIdx].pt);
		imagePoints2.push_back(_test_keypoints[matchePoints[i].trainIdx].pt);
	}
	cv::Mat homo = cv::findHomography(imagePoints2, imagePoints1, CV_RANSAC);//默认变换矩阵size为3*3

	cv::Mat imageTransform1, imageTransform2;

	warpPerspective(this->_test_dest, imageTransform1, homo, cv::Size(this->_tmpl_dest.cols, this->_tmpl_dest.rows));

	//获取配准后的差分图像
	this->_result_img = (this->_tmpl_dest ^ imageTransform1);

#ifdef SHOW_TEST_IMAGE
	//显示最符合的20个特征点与变换后的图片
	cv::Mat imageOutput;
	drawMatches(this->_tmpl_dest, _tmpl_keypoints, this->_test_dest, _test_keypoints, matchePoints, imageOutput);
	cv::namedWindow("Mathch Points", 0);
	imshow("Mathch Points", imageOutput);
	imshow("经过透视矩阵变换后", imageTransform1);
	cv::waitKey();
#endif // !SHOW_TEST_IMAGE
}

/*
** 显示配准结果（测试用）
*/
void RegistrationSift::ShowResultImg()
{
	// Show registration result
	imshow("配准后差分图像", this->_result_img);
	cv::Mat tmp_img1, tmp_img2, tmp_img3;
	cv::cvtColor(_result_img, tmp_img1, cv::COLOR_BGR2GRAY);
	//cv::threshold(tmp_img1, tmp_img2, 30, 200.0, CV_THRESH_BINARY);
	cv::erode(tmp_img1, tmp_img3,(3,3),cv::Point(-1,-1),3);
	imshow("腐蚀后差分图像", tmp_img3);
	cv::waitKey();
}

/*
cv::Mat homo = cv::getAffineTransform(imagePoints2, imagePoints1);
cv::warpAffine(this->_test_dest, imageTransform1, homo, cv::Size(this->_tmpl_dest.cols, this->_tmpl_dest.rows));
*/