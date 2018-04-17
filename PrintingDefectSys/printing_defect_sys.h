#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_printing_defect_sys.h"

#include <opencv2\highgui\highgui.hpp>  
#include "opencv2/nonfree/nonfree.hpp"  
#include "opencv2/legacy/legacy.hpp"  

class PrintingDefectSys : public QMainWindow
{
	Q_OBJECT

public:
	PrintingDefectSys(QWidget *parent = Q_NULLPTR);

private:
	Ui::PrintingDefectSysClass ui;
};
