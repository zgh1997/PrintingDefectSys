#include "registration_asift.h"
#include "registration_sift.h"
/* 
* 作用：根据变换矩阵对图像进行仿射变换
* 参数：
*/
void RegistrationASift::AffineSkew(double tilt, double phi, cv::Mat& img, cv::Mat& mask, cv::Mat& Ai)
{
	int h = img.rows;
	int w = img.cols;

	mask = cv::Mat(h, w, CV_8UC1, cv::Scalar(255));

	cv::Mat A = cv::Mat::eye(2, 3, CV_32F);

	if (phi != 0.0)
	{
		phi *= M_PI / 180.;
		double s = sin(phi);
		double c = cos(phi);

		A = (cv::Mat_<float>(2, 2) << c, -s, s, c);

		cv::Mat corners = (cv::Mat_<float>(4, 2) << 0, 0, w, 0, w, h, 0, h);
		cv::Mat tcorners = corners*A.t();
		cv::Mat tcorners_x, tcorners_y;
		tcorners.col(0).copyTo(tcorners_x);
		tcorners.col(1).copyTo(tcorners_y);
		std::vector<cv::Mat> channels;
		channels.push_back(tcorners_x);
		channels.push_back(tcorners_y);
		merge(channels, tcorners);

		cv::Rect rect = boundingRect(tcorners);
		A = (cv::Mat_<float>(2, 3) << c, -s, -rect.x, s, c, -rect.y);

		warpAffine(img, img, A, cv::Size(rect.width, rect.height), cv::INTER_LINEAR, cv::BORDER_REPLICATE);
	}
	if (tilt != 1.0)
	{
		double s = 0.8*sqrt(tilt*tilt - 1);
		GaussianBlur(img, img, cv::Size(0, 0), s, 0.01);
		resize(img, img, cv::Size(0, 0), 1.0 / tilt, 1.0, cv::INTER_NEAREST);
		A.row(0) = A.row(0) / tilt;
	}
	if (tilt != 1.0 || phi != 0.0)
	{
		h = img.rows;
		w = img.cols;
		warpAffine(mask, mask, A, cv::Size(w, h), cv::INTER_NEAREST);
	}
	invertAffineTransform(A, Ai);
}

void RegistrationASift::DetectAndCompute(const cv::Mat& img, std::vector< cv::KeyPoint >& keypoints, cv::Mat& descriptors)
{
	keypoints.clear();
	// TODO:Implement affine transform
	descriptors = cv::Mat(0, 128, CV_32F);
	// 倾斜尺度
	for (int tl = 1; tl < 6; tl++)
	{
		double t = pow(2, 0.5*tl);
		// 倾斜角度
		for (int phi = 0; phi < 180; phi += 72.0 / t)
		{
			// 临时存储特征点
			std::vector<cv::KeyPoint> kps;
			cv::Mat desc;

			cv::Mat timg, mask, Ai;
			img.copyTo(timg);
			//仿射变换
			AffineSkew(t, phi, timg, mask, Ai);
#ifdef SHOW_TEST_IMAGE
			cv::Mat img_disp;
			cv::bitwise_and(mask, timg, img_disp);
			cv::namedWindow("Skew", cv::WINDOW_AUTOSIZE);// Create a window for display.
			cv::imshow("Skew", img_disp);
			cv::waitKey();
#endif

			cv::SiftFeatureDetector detector;
			detector.detect(timg, kps, mask);

			cv::SiftDescriptorExtractor extractor;
			extractor.compute(timg, kps, desc);

			for (unsigned int i = 0; i < kps.size(); i++)
			{
				cv::Point3f kpt(kps[i].pt.x, kps[i].pt.y, 1);
				cv::Mat kpt_t = Ai*cv::Mat(kpt);
				kps[i].pt.x = kpt_t.at<float>(0, 0);
				kps[i].pt.y = kpt_t.at<float>(1, 0);
			}
			keypoints.insert(keypoints.end(), kps.begin(), kps.end());
			descriptors.push_back(desc);
		}
	}
}

/*
* 显示配准结果差分图像
*/
void RegistrationASift::ShowResultImg()
{
	cv::imshow("配准后差分图像", this->_result_img);
	cv::waitKey();
}

// 实现配准
/*
* 实现配准
*/
void RegistrationASift::RegistrateTestImg()
{
	cv::Mat tmpl_desc, test_desc;
	RegistrationSift sift_detector;
	this->_tmpl_keypoints.clear();
	this->_test_keypoints.clear();
	sift_detector.DetectFeaturePoints(_tmpl_src, _tmpl_dest, this->_tmpl_keypoints, tmpl_desc);
	sift_detector.DetectFeaturePoints(_test_src, _test_dest, this->_test_keypoints, test_desc);

	//cv::BruteForceMatcher<cv::L2<float>> matcher;
	cv::FlannBasedMatcher matcher;
	std::vector<cv::DMatch> matchePoints;
	matcher.match(tmpl_desc, test_desc, matchePoints, cv::Mat());


	// 计算投影映射矩阵
	std::vector<cv::Point2f> imagePoints1, imagePoints2;
	for (int i = 0; i < matchePoints.size(); i++) {
		imagePoints1.push_back(_tmpl_keypoints[matchePoints[i].queryIdx].pt);
		imagePoints2.push_back(_test_keypoints[matchePoints[i].trainIdx].pt);
	}
	cv::Mat homo = cv::findHomography(imagePoints2, imagePoints1, CV_RANSAC);//默认变换矩阵size为3*3

	cv::Mat imageTransform1, imageTransform2;

	warpPerspective(this->_test_dest, imageTransform1, homo, cv::Size(this->_tmpl_dest.cols, this->_tmpl_dest.rows));

	//获取配准后的差分图像
	this->_test_dest = imageTransform1;
	this->_result_img = (this->_tmpl_dest ^ imageTransform1);

}


