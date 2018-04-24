#define SHOW_TEST_IMAGE
#include "printing_defect_sys.h"
#include <QtWidgets/QApplication>
#include "registration_sift.h"


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	/* Qt UI
	SIFT_Test w;
	w.show();
	*/
	RegistrationSift p = RegistrationSift("E:\\test\\hs1.png", "E:\\test\\hs2.png");
	p.DetectFeaturePoints();
	p.RegistrateTestImg();
	p.ShowMatchedFeatures();
	return a.exec();
}
