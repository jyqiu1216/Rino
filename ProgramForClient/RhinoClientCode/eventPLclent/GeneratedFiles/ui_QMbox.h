/********************************************************************************
** Form generated from reading UI file 'QMbox.ui'
**
** Created by: Qt User Interface Compiler version 5.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QMBOX_H
#define UI_QMBOX_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QMbox
{
public:
    QWidget *widget;
    QLabel *label_content;
    QLabel *label;
    QWidget *widget_2;
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButton_OK;
    QPushButton *pushButton_NO;

    void setupUi(QWidget *QMbox)
    {
        if (QMbox->objectName().isEmpty())
            QMbox->setObjectName(QStringLiteral("QMbox"));
        QMbox->resize(372, 200);
        widget = new QWidget(QMbox);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setGeometry(QRect(0, 0, 361, 191));
        widget->setStyleSheet(QString::fromUtf8("background-image: url(:/eventq/png/\345\274\271\347\252\227\346\217\220\347\244\272/\345\274\271\347\252\227-\345\272\225.png);"));
        label_content = new QLabel(widget);
        label_content->setObjectName(QStringLiteral("label_content"));
        label_content->setGeometry(QRect(20, 60, 311, 61));
        label_content->setStyleSheet(QString::fromUtf8("background-image: url(:/eventq/png/\345\205\250\351\200\217\346\230\216.png);color: rgb(255, 255, 255);"));
        label_content->setTextFormat(Qt::AutoText);
        label_content->setAlignment(Qt::AlignCenter);
        label = new QLabel(widget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(110, 20, 131, 31));
        label->setStyleSheet(QString::fromUtf8("font: 18pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";\n"
"color: rgb(255, 255, 255);\n"
"background-image: url(:/eventq/png/\345\205\250\351\200\217\346\230\216.png);"));
        label->setAlignment(Qt::AlignCenter);
        widget_2 = new QWidget(widget);
        widget_2->setObjectName(QStringLiteral("widget_2"));
        widget_2->setGeometry(QRect(70, 120, 226, 52));
        widget_2->setStyleSheet(QString::fromUtf8("background-image: url(:/eventq/png/\345\205\250\351\200\217\346\230\216.png);"));
        gridLayout = new QGridLayout(widget_2);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        pushButton_OK = new QPushButton(widget_2);
        pushButton_OK->setObjectName(QStringLiteral("pushButton_OK"));
        pushButton_OK->setMinimumSize(QSize(100, 32));
        pushButton_OK->setMaximumSize(QSize(100, 32));
        pushButton_OK->setStyleSheet(QString::fromUtf8("background-image: url(:/eventq/png/\345\274\271\347\252\227\346\217\220\347\244\272/\345\274\271\347\252\227-\346\214\211\351\222\2561.png);\n"
"color: rgb(255, 255, 255);"));
        pushButton_OK->setCheckable(false);
        pushButton_OK->setAutoRepeat(false);
        pushButton_OK->setFlat(true);

        horizontalLayout->addWidget(pushButton_OK);

        pushButton_NO = new QPushButton(widget_2);
        pushButton_NO->setObjectName(QStringLiteral("pushButton_NO"));
        pushButton_NO->setMinimumSize(QSize(100, 32));
        pushButton_NO->setMaximumSize(QSize(100, 32));
        pushButton_NO->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-image: url(:/eventq/png/\345\274\271\347\252\227\346\217\220\347\244\272/\345\274\271\347\252\227-\346\214\211\351\222\2562.png);"));
        pushButton_NO->setFlat(true);

        horizontalLayout->addWidget(pushButton_NO);


        gridLayout->addLayout(horizontalLayout, 0, 0, 1, 1);


        retranslateUi(QMbox);
        QObject::connect(pushButton_NO, SIGNAL(clicked()), QMbox, SLOT(slots_NO()));
        QObject::connect(pushButton_OK, SIGNAL(clicked()), QMbox, SLOT(slots_OK()));

        QMetaObject::connectSlotsByName(QMbox);
    } // setupUi

    void retranslateUi(QWidget *QMbox)
    {
        QMbox->setWindowTitle(QApplication::translate("QMbox", "QMbox", 0));
        label_content->setText(QApplication::translate("QMbox", "0", 0));
        label->setText(QApplication::translate("QMbox", "\346\217\220\347\244\272", 0));
        pushButton_OK->setText(QApplication::translate("QMbox", "\347\241\256\350\256\244", 0));
        pushButton_NO->setText(QApplication::translate("QMbox", "\345\217\226\346\266\210", 0));
    } // retranslateUi

};

namespace Ui {
    class QMbox: public Ui_QMbox {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QMBOX_H
