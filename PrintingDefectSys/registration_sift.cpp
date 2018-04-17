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
** Ԥ������ȡ������
*/
void RegistrationSift::DetectFeaturePoints()
{
	// Ԥ������˹�˲�
	GaussianBlur(this->_tmpl_img, this->_tmpl_pre, cv::Size(3, 3), 0.5);
	GaussianBlur(this->_test_img, this->_test_pre, cv::Size(3, 3), 0.5);

	//��ȡ������
	cv::SiftFeatureDetector siftDetector(100);  //��ȡǰ ��������    
	siftDetector.detect(this->_tmpl_pre, this->keyPoint1);
	siftDetector.detect(this->_test_pre, this->keyPoint2);
}

/*
** ����ƥ�����׼
*/
void RegistrationSift::RegistrateTestImg()
{
	//������������Ϊ�±ߵ�������ƥ����׼��  
	cv::SiftDescriptorExtractor siftDescriptor;
	cv::Mat imageDesc1, imageDesc2;
	siftDescriptor.compute(this->_tmpl_pre, keyPoint1, imageDesc1);
	siftDescriptor.compute(this->_test_pre, keyPoint2, imageDesc2);

	//������ƥ�䲢��ʾƥ����  
	cv::BruteForceMatcher<cv::L2<float>> matcher;
	std::vector<cv::DMatch> matchePoints;
	matcher.match(imageDesc1, imageDesc2, matchePoints, cv::Mat());

	/*
	** ����ͶӰӳ����󣬽�����ͼ��׼��ģ��
	*/

	// ѡȡ����ϵ�20��������
	std::nth_element(matchePoints.begin(), matchePoints.begin() + 19, matchePoints.end());
	//matchePoints.erase(matchePoints.begin() + 20, matchePoints.end());

	// ����ͶӰӳ�����
	std::vector<cv::Point2f> imagePoints1, imagePoints2;
	for (int i = 0; i < 20; i++) {
		imagePoints1.push_back(keyPoint1[matchePoints[i].queryIdx].pt);
		imagePoints2.push_back(keyPoint2[matchePoints[i].trainIdx].pt);
	}
	cv::Mat homo = findHomography(imagePoints2, imagePoints1, CV_RANSAC);//Ĭ�ϱ任����sizeΪ3*3

	cv::Mat imageTransform1, imageTransform2;
	warpPerspective(this->_test_pre, imageTransform1, homo, cv::Size(this->_tmpl_pre.cols, this->_tmpl_pre.rows));

#ifdef SHOW_TEST_IMAGE
	//��ʾ����ϵ�20����������任���ͼƬ
	cv::Mat imageOutput;
	drawMatches(this->_tmpl_pre, keyPoint1, this->_test_pre, keyPoint2, matchePoints, imageOutput);
	cv::namedWindow("Mathch Points", 0);
	imshow("Mathch Points", imageOutput);
	cv::waitKey();

	imshow("����͸�Ӿ���任��", imageTransform1);
#endif // !SHOW_TEST_IMAGE
}

/*
** ��ʾ��׼����������ã�
*/
void RegistrationSift::ShowMatchedFeatures()
{
	//����������  
	drawKeypoints(this->_tmpl_pre, keyPoint1, this->_tmpl_pre, cv::Scalar::all(-1), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
	drawKeypoints(this->_test_pre, keyPoint2, this->_test_pre, cv::Scalar::all(-1), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
	cv::namedWindow("KeyPoints of tmpl_pre", 0);
	cv::namedWindow("KeyPoints of test_pre", 0);

	imshow("KeyPoints of tmpl_pre", this->_tmpl_pre);
	imshow("KeyPoints of test_pre", this->_test_pre);


	//������������Ϊ�±ߵ�������ƥ����׼��  
	cv::SiftDescriptorExtractor siftDescriptor;
	cv::Mat imageDesc1, imageDesc2;
	siftDescriptor.compute(this->_tmpl_pre, keyPoint1, imageDesc1);
	siftDescriptor.compute(this->_test_pre, keyPoint2, imageDesc2);

	//������ƥ�䲢��ʾƥ����  
	cv::BruteForceMatcher<cv::L2<float>> matcher;
	std::vector<cv::DMatch> matchePoints;
	matcher.match(imageDesc1, imageDesc2, matchePoints, cv::Mat());
	cv::Mat imageOutput;
	drawMatches(this->_tmpl_img, keyPoint1, this->_test_img, keyPoint2, matchePoints, imageOutput);
	cv::namedWindow("Mathch Points", 0);
	imshow("Mathch Points", imageOutput);
	cv::waitKey();
}