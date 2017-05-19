/********************************************************************************
** Form generated from reading UI file 'ClientLink.ui'
**
** Created by: Qt User Interface Compiler version 5.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CLIENTLINK_H
#define UI_CLIENTLINK_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ClientLink
{
public:
    QWidget *widget;
    QLabel *label;
    QPushButton *pushButton_close;
    QPushButton *pushButton_Reconnect;

    void setupUi(QDialog *ClientLink)
    {
        if (ClientLink->objectName().isEmpty())
            ClientLink->setObjectName(QStringLiteral("ClientLink"));
        ClientLink->resize(323, 192);
        widget = new QWidget(ClientLink);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setGeometry(QRect(10, 10, 301, 171));
        widget->setStyleSheet(QString::fromUtf8("background-image: url(:/eventq/png/\345\233\242\351\230\237\346\214\221\346\210\230\350\265\233background.png);"));
        label = new QLabel(widget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(50, 50, 221, 31));
        label->setStyleSheet(QLatin1String("color: rgb(255, 255, 255);\n"
"font: 75 14pt \"Arial\";"));
        pushButton_close = new QPushButton(widget);
        pushButton_close->setObjectName(QStringLiteral("pushButton_close"));
        pushButton_close->setGeometry(QRect(270, 10, 11, 9));
        pushButton_close->setStyleSheet(QString::fromUtf8("border-image: url(:/eventq/png/\345\205\263\351\227\255.png);"));
        pushButton_close->setFlat(true);
        pushButton_Reconnect = new QPushButton(widget);
        pushButton_Reconnect->setObjectName(QStringLiteral("pushButton_Reconnect"));
        pushButton_Reconnect->setGeometry(QRect(100, 100, 91, 31));
        pushButton_Reconnect->setStyleSheet(QString::fromUtf8("border-image: url(:/eventq/png/\344\270\252\344\272\272\350\265\204\346\226\231\345\210\207\345\233\276/\344\277\235\345\255\230\346\214\211\351\222\256.png);\n"
"color: rgb(255, 255, 255);"));
        pushButton_Reconnect->setFlat(true);

        retranslateUi(ClientLink);
        QObject::connect(pushButton_close, SIGNAL(clicked()), ClientLink, SLOT(close()));

        QMetaObject::connectSlotsByName(ClientLink);
    } // setupUi

    void retranslateUi(QDialog *ClientLink)
    {
        ClientLink->setWindowTitle(QApplication::translate("ClientLink", "ClientLink", 0));
        label->setText(QApplication::translate("ClientLink", "\346\255\243\345\234\250\345\220\257\345\212\250\350\257\267\347\250\215\345\220\216...........", 0));
        pushButton_close->setText(QString());
        pushButton_Reconnect->setText(QApplication::translate("ClientLink", "\351\207\215\346\226\260\350\277\236\346\216\245", 0));
    } // retranslateUi

};

namespace Ui {
    class ClientLink: public Ui_ClientLink {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CLIENTLINK_H
