#include "registration_sift.h"

/*
** 预处理及提取特征点
*/
void RegistrationSift::DetectFeaturePoints()
{
	// 预处理：高斯滤波
	GaussianBlur(this->_tmpl_src, this->_tmpl_dest, cv::Size(3, 3), 0.5);
	GaussianBlur(this->_test_src, this->_test_dest, cv::Size(3, 3), 0.5);

	//提取特征点
	cv::SiftFeatureDetector siftDetector(200);  //提取前 个特征点    
	siftDetector.detect(this->_tmpl_dest, this->keyPoint1);
	siftDetector.detect(this->_test_dest, this->keyPoint2);
}

/*
** 根据匹配点配准
*/
void RegistrationSift::RegistrateTestImg()
{
	//特征点描述，为下边的特征点匹配做准备  
	cv::SiftDescriptorExtractor siftDescriptor;
	cv::Mat imageDesc1, imageDesc2;
	siftDescriptor.compute(this->_tmpl_dest, keyPoint1, imageDesc1);
	siftDescriptor.compute(this->_test_dest, keyPoint2, imageDesc2);

	//特征点匹配并显示匹配结果  
	cv::BruteForceMatcher<cv::L2<float>> matcher;
	std::vector<cv::DMatch> matchePoints;
	matcher.match(imageDesc1, imageDesc2, matchePoints, cv::Mat());

	/*
	** 计算投影映射矩阵，将测试图配准到模板
	*/

	// 选取最符合的20个特征点,舍去其他点
	std::nth_element(matchePoints.begin(), matchePoints.begin() + 99, matchePoints.end());
	//matchePoints.erase(matchePoints.begin() + 100, matchePoints.end());

	// 计算投影映射矩阵
	std::vector<cv::Point2f> imagePoints1, imagePoints2;
	for (int i = 0; i < 100; i++) {
		imagePoints1.push_back(keyPoint1[matchePoints[i].queryIdx].pt);
		imagePoints2.push_back(keyPoint2[matchePoints[i].trainIdx].pt);
	}
	cv::Mat homo = cv::findHomography(imagePoints2, imagePoints1, CV_RANSAC);//默认变换矩阵size为3*3

	cv::Mat imageTransform1, imageTransform2;

	warpPerspective(this->_test_dest, imageTransform1, homo, cv::Size(this->_tmpl_dest.cols, this->_tmpl_dest.rows));

	//获取配准后的差分图像
	this->_result_img = (this->_tmpl_dest ^ imageTransform1);

#ifdef SHOW_TEST_IMAGE
	//显示最符合的20个特征点与变换后的图片
	cv::Mat imageOutput;
	drawMatches(this->_tmpl_dest, keyPoint1, this->_test_dest, keyPoint2, matchePoints, imageOutput);
	cv::namedWindow("Mathch Points", 0);
	imshow("Mathch Points", imageOutput);
	imshow("经过透视矩阵变换后", imageTransform1);
	cv::waitKey();
#endif // !SHOW_TEST_IMAGE
}

/*
** 显示配准结果（测试用）
*/
void RegistrationSift::ShowMatchedFeatures()
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