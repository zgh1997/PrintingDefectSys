#include "utils_registration.h"


/*
* 画匹配的特征点对
*/
void UtilsRegistration::DrawMatch(const cv::Mat &src, const cv::Mat &obj, std::vector<cv::Point2f> &srcPoints, std::vector<cv::Point2f> &dstPoints) {
	// Create a image for displaying mathing keypoints
	cv::Size sz = cv::Size(src.size().width + obj.size().width, src.size().height + obj.size().height);
	cv::Mat matchingImage = cv::Mat::zeros(sz, CV_8UC3);

	// Draw camera frame
	cv::Mat roi1 = cv::Mat(matchingImage, cv::Rect(0, 0, src.size().width, src.size().height));
	src.copyTo(roi1);
	// Draw original image
	cv::Mat roi2 = cv::Mat(matchingImage, cv::Rect(src.size().width, src.size().height, obj.size().width, obj.size().height));
	obj.copyTo(roi2);

	// Draw line between nearest neighbor pairs
	for (int i = 0; i < (int)srcPoints.size(); ++i) {
		cv::Point2f pt1 = srcPoints[i];
		cv::Point2f pt2 = dstPoints[i];
		cv::Point2f from = pt1;
		cv::Point2f to = cv::Point(src.size().width + pt2.x, src.size().height + pt2.y);
		line(matchingImage, from, to, cv::Scalar(0, 255, 255));
	}
	// Display mathing image
	resize(matchingImage, matchingImage, cv::Size(matchingImage.cols / 2, matchingImage.rows / 2));
	cv::namedWindow("Display frame", CV_WINDOW_AUTOSIZE);
	imshow("Matched Points", matchingImage);
	cvWaitKey(0);
}

void UtilsRegistration::FindInliers(std::vector<cv::KeyPoint> &qKeypoints, std::vector<cv::KeyPoint> &objKeypoints, std::vector<cv::DMatch> &matches, const cv::Mat &src, const cv::Mat &obj) {
	// 获取关键点坐标
	std::vector<cv::Point2f> queryCoord;
	std::vector<cv::Point2f> objectCoord;
	for (int i = 0; i < matches.size(); i++) {
		queryCoord.push_back((qKeypoints[matches[i].queryIdx]).pt);
		objectCoord.push_back((objKeypoints[matches[i].trainIdx]).pt);
	}
	// 使用自定义的函数显示匹配点对
	DrawMatch(src, obj, queryCoord, objectCoord);

	// 计算homography矩阵
	cv::Mat mask;
	std::vector<cv::Point2f> queryInliers;
	std::vector<cv::Point2f> sceneInliers;
	cv::Mat H = findFundamentalMat(queryCoord, objectCoord, mask, CV_FM_RANSAC);
	//Mat H = findHomography( queryCoord, objectCoord, CV_RANSAC, 10, mask);
	int inliers_cnt = 0, outliers_cnt = 0;
	for (int j = 0; j < mask.rows; j++) {
		if (mask.at<uchar>(j) == 1) {
			queryInliers.push_back(queryCoord[j]);
			sceneInliers.push_back(objectCoord[j]);
			inliers_cnt++;
		}
		else {
			outliers_cnt++;
		}
	}
	//显示剔除误配点对后的匹配点对
	DrawMatch(src, obj, queryInliers, sceneInliers);
}