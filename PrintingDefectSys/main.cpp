#define SHOW_TEST_IMAGE
#include "printing_defect_sys.h"
#include <QtWidgets/QApplication>
#include "registration_asift.h"
#include "registration_sift.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	/* Qt UI
	SIFT_Test w;
	w.show();
	*/
	RegistrationASift asift_p = RegistrationASift("E:\\test\\test1.png", "E:\\test\\test2.png");
	asift_p.RegistrateTestImg();


	return a.exec();
}
