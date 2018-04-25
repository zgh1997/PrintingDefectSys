#include "registration_sift.h"
#define FEATURE_POINT_NUM 200
//#define SHOW_TEST_IMAGE

/*******�����ⲿ����*******/
/**************************/
/*
 * ���img��˹�˲�����Ӧ�Ĺؼ���
*/
void RegistrationSift::DetectFeaturePoints(const cv::Mat& img, cv::Mat& dest, std::vector<cv::KeyPoint>& keypoints , cv::Mat& descriptors)
{
	// Ԥ������˹�˲�,����׼ȷ��ȡ�߶Ȳ�������
	GaussianBlur(img, dest, cv::Size(3, 3), 0.5);
	//��ȡ������
	std::vector<cv::KeyPoint> points;
	cv::SiftFeatureDetector siftDetector(FEATURE_POINT_NUM);  //��ȡǰFEATURE_POINT_NUM��������    
	siftDetector.detect(dest, points);

	cv::SiftDescriptorExtractor extractor;
	extractor.compute(img, points, descriptors);
	keypoints.insert(keypoints.end(), points.begin(), points.end());

}



/*******������ͼ�����ϵ�в���*******/
/**************************/

/*
** ����ƥ�����׼
*/
void RegistrationSift::RegistrateTestImg()
{
	//������������Ϊ�±ߵ�������ƥ����׼��  
	cv::SiftDescriptorExtractor siftDescriptor;
	cv::Mat imageDesc1, imageDesc2;
	_tmpl_keypoints.clear();
	_test_keypoints.clear();
	this->DetectFeaturePoints(this->_tmpl_src, this->_tmpl_dest, this->_tmpl_keypoints, imageDesc1);
	this->DetectFeaturePoints(this->_test_src, this->_test_dest, this->_test_keypoints, imageDesc2);

	//������ƥ�䲢��ʾƥ����  
	cv::BruteForceMatcher<cv::L2<float>> matcher;
	std::vector<cv::DMatch> matchePoints;
	matcher.match(imageDesc1, imageDesc2, matchePoints, cv::Mat());

	/*
	** ����ͶӰӳ����󣬽�����ͼ��׼��ģ��
	*/

	//// ѡȡ����ϵ�50��������,��ȥ������
	//std::nth_element(matchePoints.begin(), matchePoints.begin() + 49, matchePoints.end());
	////matchePoints.erase(matchePoints.begin() + 100, matchePoints.end());

	// ����ͶӰӳ�����
	std::vector<cv::Point2f> imagePoints1, imagePoints2;
	for (int i = 0; i < FEATURE_POINT_NUM; i++) {
		imagePoints1.push_back(_tmpl_keypoints[matchePoints[i].queryIdx].pt);
		imagePoints2.push_back(_test_keypoints[matchePoints[i].trainIdx].pt);
	}
	cv::Mat homo = cv::findHomography(imagePoints2, imagePoints1, CV_RANSAC);//Ĭ�ϱ任����sizeΪ3*3

	cv::Mat imageTransform1, imageTransform2;

	warpPerspective(this->_test_dest, imageTransform1, homo, cv::Size(this->_tmpl_dest.cols, this->_tmpl_dest.rows));

	//��ȡ��׼��Ĳ��ͼ��
	this->_result_img = (this->_tmpl_dest ^ imageTransform1);

#ifdef SHOW_TEST_IMAGE
	//��ʾ����ϵ�20����������任���ͼƬ
	cv::Mat imageOutput;
	drawMatches(this->_tmpl_dest, _tmpl_keypoints, this->_test_dest, _test_keypoints, matchePoints, imageOutput);
	cv::namedWindow("Mathch Points", 0);
	imshow("Mathch Points", imageOutput);
	imshow("����͸�Ӿ���任��", imageTransform1);
	cv::waitKey();
#endif // !SHOW_TEST_IMAGE
}

/*
** ��ʾ��׼����������ã�
*/
void RegistrationSift::ShowResultImg()
{
	// Show registration result
	imshow("��׼����ͼ��", this->_result_img);
	cv::Mat tmp_img1, tmp_img2, tmp_img3;
	cv::cvtColor(_result_img, tmp_img1, cv::COLOR_BGR2GRAY);
	//cv::threshold(tmp_img1, tmp_img2, 30, 200.0, CV_THRESH_BINARY);
	cv::erode(tmp_img1, tmp_img3,(3,3),cv::Point(-1,-1),3);
	imshow("��ʴ����ͼ��", tmp_img3);
	cv::waitKey();
}

/*
cv::Mat homo = cv::getAffineTransform(imagePoints2, imagePoints1);
cv::warpAffine(this->_test_dest, imageTransform1, homo, cv::Size(this->_tmpl_dest.cols, this->_tmpl_dest.rows));
*/