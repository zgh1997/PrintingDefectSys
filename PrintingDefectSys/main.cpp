#define SHOW_TEST_IMAGE
#include "printing_defect_sys.h"
#include <QtWidgets/QApplication>
#include "registration_asift.h"
#include "registration_sift.h"
#include "utils_pretreat.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	/* Qt UI
	SIFT_Test w;
	w.show();
	*/

	/*
	RegistrationASift asift_p = RegistrationASift("E:\\test\\test1.png", "E:\\test\\test2.png");

	asift_p.RegistrateTestImg();
	asift_p.ShowResultImg();
	*/


	RegistrationSift sift_p = RegistrationSift("E:\\test\\test1.png", "E:\\test\\test2.png");

	sift_p.RegistrateTestImg();
	sift_p.ShowResultImg();


	/*
	cv::Mat dif_img;
	dif_img = sift_p._tmpl_dest ^ sift_p._test_dest;
	cv::namedWindow("Dif", CV_WINDOW_AUTOSIZE);
	cv::imshow("Dif", dif_img);
	*/

	UtilsPretreat pretreat_p;
	cv::Mat threshold_img;
	pretreat_p.Threshold(sift_p._result_img, threshold_img);
	return a.exec();
}
