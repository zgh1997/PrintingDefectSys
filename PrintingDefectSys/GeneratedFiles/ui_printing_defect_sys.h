/********************************************************************************
** Form generated from reading UI file 'printing_defect_sys.ui'
**
** Created by: Qt User Interface Compiler version 5.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PRINTING_DEFECT_SYS_H
#define UI_PRINTING_DEFECT_SYS_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PrintingDefectSysClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *PrintingDefectSysClass)
    {
        if (PrintingDefectSysClass->objectName().isEmpty())
            PrintingDefectSysClass->setObjectName(QStringLiteral("PrintingDefectSysClass"));
        PrintingDefectSysClass->resize(600, 400);
        menuBar = new QMenuBar(PrintingDefectSysClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        PrintingDefectSysClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(PrintingDefectSysClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        PrintingDefectSysClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(PrintingDefectSysClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        PrintingDefectSysClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(PrintingDefectSysClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        PrintingDefectSysClass->setStatusBar(statusBar);

        retranslateUi(PrintingDefectSysClass);

        QMetaObject::connectSlotsByName(PrintingDefectSysClass);
    } // setupUi

    void retranslateUi(QMainWindow *PrintingDefectSysClass)
    {
        PrintingDefectSysClass->setWindowTitle(QApplication::translate("PrintingDefectSysClass", "PrintingDefectSys", 0));
    } // retranslateUi

};

namespace Ui {
    class PrintingDefectSysClass: public Ui_PrintingDefectSysClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PRINTING_DEFECT_SYS_H
