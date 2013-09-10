/********************************************************************************
** Form generated from reading UI file 'nkcsolution.ui'
**
** Created by: Qt User Interface Compiler version 5.1.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NKCSOLUTION_H
#define UI_NKCSOLUTION_H

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

class Ui_NKCSolutionClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *NKCSolutionClass)
    {
        if (NKCSolutionClass->objectName().isEmpty())
            NKCSolutionClass->setObjectName(QStringLiteral("NKCSolutionClass"));
        NKCSolutionClass->resize(600, 400);
        menuBar = new QMenuBar(NKCSolutionClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        NKCSolutionClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(NKCSolutionClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        NKCSolutionClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(NKCSolutionClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        NKCSolutionClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(NKCSolutionClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        NKCSolutionClass->setStatusBar(statusBar);

        retranslateUi(NKCSolutionClass);

        QMetaObject::connectSlotsByName(NKCSolutionClass);
    } // setupUi

    void retranslateUi(QMainWindow *NKCSolutionClass)
    {
        NKCSolutionClass->setWindowTitle(QApplication::translate("NKCSolutionClass", "NKCSolution", 0));
    } // retranslateUi

};

namespace Ui {
    class NKCSolutionClass: public Ui_NKCSolutionClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NKCSOLUTION_H
