/********************************************************************************
** Form generated from reading UI file 'webpopuppag.ui'
**
** Created by: Qt User Interface Compiler version 5.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WEBPOPUPPAG_H
#define UI_WEBPOPUPPAG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>

QT_BEGIN_NAMESPACE

class Ui_webpopuppag
{
public:
    QGridLayout *gridLayout_2;
    QGridLayout *gridLayout;

    void setupUi(QDialog *webpopuppag)
    {
        if (webpopuppag->objectName().isEmpty())
            webpopuppag->setObjectName(QStringLiteral("webpopuppag"));
        webpopuppag->resize(400, 300);
        gridLayout_2 = new QGridLayout(webpopuppag);
        gridLayout_2->setSpacing(0);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        gridLayout = new QGridLayout();
        gridLayout->setSpacing(0);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setSizeConstraint(QLayout::SetNoConstraint);

        gridLayout_2->addLayout(gridLayout, 0, 0, 1, 1);


        retranslateUi(webpopuppag);

        QMetaObject::connectSlotsByName(webpopuppag);
    } // setupUi

    void retranslateUi(QDialog *webpopuppag)
    {
        webpopuppag->setWindowTitle(QApplication::translate("webpopuppag", "webpopuppag", 0));
    } // retranslateUi

};

namespace Ui {
    class webpopuppag: public Ui_webpopuppag {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WEBPOPUPPAG_H
