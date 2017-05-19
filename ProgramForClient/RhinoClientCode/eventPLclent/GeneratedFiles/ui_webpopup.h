/********************************************************************************
** Form generated from reading UI file 'webpopup.ui'
**
** Created by: Qt User Interface Compiler version 5.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WEBPOPUP_H
#define UI_WEBPOPUP_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_webPopup
{
public:
    QGridLayout *gridLayout_2;
    QGridLayout *gridLayout;

    void setupUi(QWidget *webPopup)
    {
        if (webPopup->objectName().isEmpty())
            webPopup->setObjectName(QStringLiteral("webPopup"));
        webPopup->resize(1739, 1001);
        webPopup->setStyleSheet(QStringLiteral("background-image: url(:/eventq/png/background.png);"));
        gridLayout_2 = new QGridLayout(webPopup);
        gridLayout_2->setSpacing(0);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        gridLayout = new QGridLayout();
        gridLayout->setSpacing(0);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setSizeConstraint(QLayout::SetNoConstraint);

        gridLayout_2->addLayout(gridLayout, 0, 0, 1, 1);


        retranslateUi(webPopup);

        QMetaObject::connectSlotsByName(webPopup);
    } // setupUi

    void retranslateUi(QWidget *webPopup)
    {
        webPopup->setWindowTitle(QApplication::translate("webPopup", "webPopup", 0));
    } // retranslateUi

};

namespace Ui {
    class webPopup: public Ui_webPopup {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WEBPOPUP_H
