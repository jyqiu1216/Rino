/********************************************************************************
** Form generated from reading UI file 'WJMainWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WJMAINWINDOW_H
#define UI_WJMAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_WJMainWindow
{
public:
    QWidget *centralWidget;

    void setupUi(QMainWindow *WJMainWindow)
    {
        if (WJMainWindow->objectName().isEmpty())
            WJMainWindow->setObjectName(QStringLiteral("WJMainWindow"));
        WJMainWindow->resize(912, 571);
        QIcon icon;
        icon.addFile(QStringLiteral("../png/dkl.ico"), QSize(), QIcon::Normal, QIcon::Off);
        WJMainWindow->setWindowIcon(icon);
        centralWidget = new QWidget(WJMainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        WJMainWindow->setCentralWidget(centralWidget);

        retranslateUi(WJMainWindow);

        QMetaObject::connectSlotsByName(WJMainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *WJMainWindow)
    {
        WJMainWindow->setWindowTitle(QApplication::translate("WJMainWindow", "\347\212\200\347\211\233\347\224\265\347\253\236", 0));
    } // retranslateUi

};

namespace Ui {
    class WJMainWindow: public Ui_WJMainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WJMAINWINDOW_H
