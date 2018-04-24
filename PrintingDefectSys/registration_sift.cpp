#include "registration_sift.h"

/*
** Ԥ������ȡ������
*/
void RegistrationSift::DetectFeaturePoints()
{
	// Ԥ������˹�˲�
	GaussianBlur(this->_tmpl_src, this->_tmpl_dest, cv::Size(3, 3), 0.5);
	GaussianBlur(this->_test_src, this->_test_dest, cv::Size(3, 3), 0.5);

	//��ȡ������
	cv::SiftFeatureDetector siftDetector(200);  //��ȡǰ ��������    
	siftDetector.detect(this->_tmpl_dest, this->keyPoint1);
	siftDetector.detect(this->_test_dest, this->keyPoint2);
}

/*
** ����ƥ�����׼
*/
void RegistrationSift::RegistrateTestImg()
{
	//������������Ϊ�±ߵ�������ƥ����׼��  
	cv::SiftDescriptorExtractor siftDescriptor;
	cv::Mat imageDesc1, imageDesc2;
	siftDescriptor.compute(this->_tmpl_dest, keyPoint1, imageDesc1);
	siftDescriptor.compute(this->_test_dest, keyPoint2, imageDesc2);

	//������ƥ�䲢��ʾƥ����  
	cv::BruteForceMatcher<cv::L2<float>> matcher;
	std::vector<cv::DMatch> matchePoints;
	matcher.match(imageDesc1, imageDesc2, matchePoints, cv::Mat());

	/*
	** ����ͶӰӳ����󣬽�����ͼ��׼��ģ��
	*/

	// ѡȡ����ϵ�20��������,��ȥ������
	std::nth_element(matchePoints.begin(), matchePoints.begin() + 99, matchePoints.end());
	//matchePoints.erase(matchePoints.begin() + 100, matchePoints.end());

	// ����ͶӰӳ�����
	std::vector<cv::Point2f> imagePoints1, imagePoints2;
	for (int i = 0; i < 100; i++) {
		imagePoints1.push_back(keyPoint1[matchePoints[i].queryIdx].pt);
		imagePoints2.push_back(keyPoint2[matchePoints[i].trainIdx].pt);
	}
	cv::Mat homo = cv::findHomography(imagePoints2, imagePoints1, CV_RANSAC);//Ĭ�ϱ任����sizeΪ3*3

	cv::Mat imageTransform1, imageTransform2;

	warpPerspective(this->_test_dest, imageTransform1, homo, cv::Size(this->_tmpl_dest.cols, this->_tmpl_dest.rows));

	//��ȡ��׼��Ĳ��ͼ��
	this->_result_img = (this->_tmpl_dest ^ imageTransform1);

#ifdef SHOW_TEST_IMAGE
	//��ʾ����ϵ�20����������任���ͼƬ
	cv::Mat imageOutput;
	drawMatches(this->_tmpl_dest, keyPoint1, this->_test_dest, keyPoint2, matchePoints, imageOutput);
	cv::namedWindow("Mathch Points", 0);
	imshow("Mathch Points", imageOutput);
	imshow("����͸�Ӿ���任��", imageTransform1);
	cv::waitKey();
#endif // !SHOW_TEST_IMAGE
}

/*
** ��ʾ��׼����������ã�
*/
void RegistrationSift::ShowMatchedFeatures()
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