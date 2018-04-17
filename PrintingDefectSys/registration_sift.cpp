#include "registration_sift.h"

// Construct func 
RegistrationSift::RegistrationSift(cv::Mat const &tmpl, cv::Mat &test)
{
	this->_tmpl_img = tmpl;
	this->_test_img = test;
}
RegistrationSift::RegistrationSift(std::string const &tmpl_file, std::string const &test_file)
{
	this->_tmpl_img = cv::imread(tmpl_file);
	this->_test_img = cv::imread(test_file);

}

/*
** 预处理及提取特征点
*/
void RegistrationSift::DetectFeaturePoints()
{
	// 预处理：高斯滤波
	GaussianBlur(this->_tmpl_img, this->_tmpl_pre, cv::Size(3, 3), 0.5);
	GaussianBlur(this->_test_img, this->_test_pre, cv::Size(3, 3), 0.5);

	//提取特征点
	cv::SiftFeatureDetector siftDetector(100);  //提取前 个特征点    
	siftDetector.detect(this->_tmpl_pre, this->keyPoint1);
	siftDetector.detect(this->_test_pre, this->keyPoint2);
}

/*
** 根据匹配点配准
*/
void RegistrationSift::RegistrateTestImg()
{
	//特征点描述，为下边的特征点匹配做准备  
	cv::SiftDescriptorExtractor siftDescriptor;
	cv::Mat imageDesc1, imageDesc2;
	siftDescriptor.compute(this->_tmpl_pre, keyPoint1, imageDesc1);
	siftDescriptor.compute(this->_test_pre, keyPoint2, imageDesc2);

	//特征点匹配并显示匹配结果  
	cv::BruteForceMatcher<cv::L2<float>> matcher;
	std::vector<cv::DMatch> matchePoints;
	matcher.match(imageDesc1, imageDesc2, matchePoints, cv::Mat());

	/*
	** 计算投影映射矩阵，将测试图配准到模板
	*/

	// 选取最符合的20个特征点
	std::nth_element(matchePoints.begin(), matchePoints.begin() + 19, matchePoints.end());
	//matchePoints.erase(matchePoints.begin() + 20, matchePoints.end());

	// 计算投影映射矩阵
	std::vector<cv::Point2f> imagePoints1, imagePoints2;
	for (int i = 0; i < 20; i++) {
		imagePoints1.push_back(keyPoint1[matchePoints[i].queryIdx].pt);
		imagePoints2.push_back(keyPoint2[matchePoints[i].trainIdx].pt);
	}
	cv::Mat homo = findHomography(imagePoints2, imagePoints1, CV_RANSAC);//默认变换矩阵size为3*3

	cv::Mat imageTransform1, imageTransform2;
	warpPerspective(this->_test_pre, imageTransform1, homo, cv::Size(this->_tmpl_pre.cols, this->_tmpl_pre.rows));

#ifdef SHOW_TEST_IMAGE
	//显示最符合的20个特征点与变换后的图片
	cv::Mat imageOutput;
	drawMatches(this->_tmpl_pre, keyPoint1, this->_test_pre, keyPoint2, matchePoints, imageOutput);
	cv::namedWindow("Mathch Points", 0);
	imshow("Mathch Points", imageOutput);
	cv::waitKey();

	imshow("经过透视矩阵变换后", imageTransform1);
#endif // !SHOW_TEST_IMAGE
}

/*
** 显示配准结果（测试用）
*/
void RegistrationSift::ShowMatchedFeatures()
{
	//绘制特征点  
	drawKeypoints(this->_tmpl_pre, keyPoint1, this->_tmpl_pre, cv::Scalar::all(-1), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
	drawKeypoints(this->_test_pre, keyPoint2, this->_test_pre, cv::Scalar::all(-1), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
	cv::namedWindow("KeyPoints of tmpl_pre", 0);
	cv::namedWindow("KeyPoints of test_pre", 0);

	imshow("KeyPoints of tmpl_pre", this->_tmpl_pre);
	imshow("KeyPoints of test_pre", this->_test_pre);


	//特征点描述，为下边的特征点匹配做准备  
	cv::SiftDescriptorExtractor siftDescriptor;
	cv::Mat imageDesc1, imageDesc2;
	siftDescriptor.compute(this->_tmpl_pre, keyPoint1, imageDesc1);
	siftDescriptor.compute(this->_test_pre, keyPoint2, imageDesc2);

	//特征点匹配并显示匹配结果  
	cv::BruteForceMatcher<cv::L2<float>> matcher;
	std::vector<cv::DMatch> matchePoints;
	matcher.match(imageDesc1, imageDesc2, matchePoints, cv::Mat());
	cv::Mat imageOutput;
	drawMatches(this->_tmpl_img, keyPoint1, this->_test_img, keyPoint2, matchePoints, imageOutput);
	cv::namedWindow("Mathch Points", 0);
	imshow("Mathch Points", imageOutput);
	cv::waitKey();
}