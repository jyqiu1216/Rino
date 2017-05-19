/********************************************************************************
** Form generated from reading UI file 'AMTest.ui'
**
** Created by: Qt User Interface Compiler version 5.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AMTEST_H
#define UI_AMTEST_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindowAMTest
{
public:
    QWidget *centralWidget;
    QComboBox *comboBox;
    QPushButton *pushButton_reloadLua;
    QGroupBox *groupBox_LOL;
    QPushButton *pushButton_lol_startup;
    QPushButton *pushButton_lol_create;
    QPushButton *pushButton_lol_join;
    QPushButton *pushButton_lol_play;
    QGroupBox *groupBox_HS;
    QPushButton *pushButton_hs_reqFrient;
    QPushButton *pushButton_hs_reqChallenge;
    QPushButton *pushButton_hs_acceptChallenge;
    QPushButton *pushButton_hs_acceptFriend;
    QPushButton *pushButton_hs_injectDll;
    QPushButton *pushButton_hs_msg_2005;
    QPushButton *pushButton_hs_msg_2006;
    QPushButton *pushButton_hs_pipei;
    QPushButton *pushButton_hs_end;
    QPushButton *pushButton_test;
    QLabel *label;
    QLineEdit *lineEdit_param_1;
    QLineEdit *lineEdit_param_2;
    QLabel *label_2;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindowAMTest)
    {
        if (MainWindowAMTest->objectName().isEmpty())
            MainWindowAMTest->setObjectName(QStringLiteral("MainWindowAMTest"));
        MainWindowAMTest->resize(372, 648);
        centralWidget = new QWidget(MainWindowAMTest);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        comboBox = new QComboBox(centralWidget);
        comboBox->setObjectName(QStringLiteral("comboBox"));
        comboBox->setGeometry(QRect(790, 40, 231, 28));
        QFont font;
        font.setFamily(QStringLiteral("YaHei Consolas Hybrid"));
        font.setPointSize(11);
        font.setBold(false);
        font.setWeight(50);
        comboBox->setFont(font);
        pushButton_reloadLua = new QPushButton(centralWidget);
        pushButton_reloadLua->setObjectName(QStringLiteral("pushButton_reloadLua"));
        pushButton_reloadLua->setGeometry(QRect(20, 110, 151, 50));
        pushButton_reloadLua->setFont(font);
        groupBox_LOL = new QGroupBox(centralWidget);
        groupBox_LOL->setObjectName(QStringLiteral("groupBox_LOL"));
        groupBox_LOL->setGeometry(QRect(10, 170, 171, 381));
        groupBox_LOL->setFont(font);
        pushButton_lol_startup = new QPushButton(groupBox_LOL);
        pushButton_lol_startup->setObjectName(QStringLiteral("pushButton_lol_startup"));
        pushButton_lol_startup->setGeometry(QRect(10, 50, 150, 50));
        pushButton_lol_startup->setFont(font);
        pushButton_lol_create = new QPushButton(groupBox_LOL);
        pushButton_lol_create->setObjectName(QStringLiteral("pushButton_lol_create"));
        pushButton_lol_create->setGeometry(QRect(10, 110, 150, 50));
        pushButton_lol_create->setFont(font);
        pushButton_lol_join = new QPushButton(groupBox_LOL);
        pushButton_lol_join->setObjectName(QStringLiteral("pushButton_lol_join"));
        pushButton_lol_join->setGeometry(QRect(10, 170, 150, 50));
        pushButton_lol_join->setFont(font);
        pushButton_lol_play = new QPushButton(groupBox_LOL);
        pushButton_lol_play->setObjectName(QStringLiteral("pushButton_lol_play"));
        pushButton_lol_play->setGeometry(QRect(10, 230, 150, 50));
        pushButton_lol_play->setFont(font);
        groupBox_HS = new QGroupBox(centralWidget);
        groupBox_HS->setObjectName(QStringLiteral("groupBox_HS"));
        groupBox_HS->setGeometry(QRect(190, 170, 171, 421));
        groupBox_HS->setFont(font);
        pushButton_hs_reqFrient = new QPushButton(groupBox_HS);
        pushButton_hs_reqFrient->setObjectName(QStringLiteral("pushButton_hs_reqFrient"));
        pushButton_hs_reqFrient->setGeometry(QRect(10, 70, 150, 40));
        pushButton_hs_reqFrient->setFont(font);
        pushButton_hs_reqChallenge = new QPushButton(groupBox_HS);
        pushButton_hs_reqChallenge->setObjectName(QStringLiteral("pushButton_hs_reqChallenge"));
        pushButton_hs_reqChallenge->setGeometry(QRect(10, 150, 150, 40));
        pushButton_hs_reqChallenge->setFont(font);
        pushButton_hs_acceptChallenge = new QPushButton(groupBox_HS);
        pushButton_hs_acceptChallenge->setObjectName(QStringLiteral("pushButton_hs_acceptChallenge"));
        pushButton_hs_acceptChallenge->setGeometry(QRect(10, 190, 150, 40));
        pushButton_hs_acceptChallenge->setFont(font);
        pushButton_hs_acceptFriend = new QPushButton(groupBox_HS);
        pushButton_hs_acceptFriend->setObjectName(QStringLiteral("pushButton_hs_acceptFriend"));
        pushButton_hs_acceptFriend->setGeometry(QRect(10, 110, 150, 40));
        pushButton_hs_acceptFriend->setFont(font);
        pushButton_hs_injectDll = new QPushButton(groupBox_HS);
        pushButton_hs_injectDll->setObjectName(QStringLiteral("pushButton_hs_injectDll"));
        pushButton_hs_injectDll->setGeometry(QRect(10, 30, 150, 40));
        pushButton_hs_injectDll->setFont(font);
        pushButton_hs_msg_2005 = new QPushButton(groupBox_HS);
        pushButton_hs_msg_2005->setObjectName(QStringLiteral("pushButton_hs_msg_2005"));
        pushButton_hs_msg_2005->setGeometry(QRect(10, 330, 150, 40));
        pushButton_hs_msg_2005->setFont(font);
        pushButton_hs_msg_2006 = new QPushButton(groupBox_HS);
        pushButton_hs_msg_2006->setObjectName(QStringLiteral("pushButton_hs_msg_2006"));
        pushButton_hs_msg_2006->setGeometry(QRect(10, 370, 150, 40));
        pushButton_hs_msg_2006->setFont(font);
        pushButton_hs_pipei = new QPushButton(groupBox_HS);
        pushButton_hs_pipei->setObjectName(QStringLiteral("pushButton_hs_pipei"));
        pushButton_hs_pipei->setGeometry(QRect(10, 230, 150, 40));
        pushButton_hs_pipei->setFont(font);
        pushButton_hs_end = new QPushButton(groupBox_HS);
        pushButton_hs_end->setObjectName(QStringLiteral("pushButton_hs_end"));
        pushButton_hs_end->setGeometry(QRect(10, 270, 150, 40));
        pushButton_hs_end->setFont(font);
        pushButton_test = new QPushButton(centralWidget);
        pushButton_test->setObjectName(QStringLiteral("pushButton_test"));
        pushButton_test->setGeometry(QRect(200, 110, 151, 50));
        pushButton_test->setFont(font);
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(30, 21, 54, 30));
        QFont font1;
        font1.setFamily(QStringLiteral("YaHei Consolas Hybrid"));
        font1.setPointSize(11);
        font1.setBold(true);
        font1.setWeight(75);
        label->setFont(font1);
        lineEdit_param_1 = new QLineEdit(centralWidget);
        lineEdit_param_1->setObjectName(QStringLiteral("lineEdit_param_1"));
        lineEdit_param_1->setGeometry(QRect(80, 20, 271, 30));
        lineEdit_param_1->setFont(font1);
        lineEdit_param_2 = new QLineEdit(centralWidget);
        lineEdit_param_2->setObjectName(QStringLiteral("lineEdit_param_2"));
        lineEdit_param_2->setGeometry(QRect(80, 59, 271, 30));
        lineEdit_param_2->setFont(font1);
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(30, 60, 54, 30));
        label_2->setFont(font1);
        MainWindowAMTest->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindowAMTest);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 372, 23));
        MainWindowAMTest->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindowAMTest);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindowAMTest->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindowAMTest);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindowAMTest->setStatusBar(statusBar);

        retranslateUi(MainWindowAMTest);

        QMetaObject::connectSlotsByName(MainWindowAMTest);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindowAMTest)
    {
        MainWindowAMTest->setWindowTitle(QApplication::translate("MainWindowAMTest", "AMTest", 0));
        pushButton_reloadLua->setText(QApplication::translate("MainWindowAMTest", "\351\207\215\346\226\260\345\212\240\350\275\275LUA", 0));
        groupBox_LOL->setTitle(QApplication::translate("MainWindowAMTest", "\350\213\261\351\233\204\350\201\224\347\233\237", 0));
        pushButton_lol_startup->setText(QApplication::translate("MainWindowAMTest", "LOL:\345\220\257\345\212\250\346\270\270\346\210\217", 0));
        pushButton_lol_create->setText(QApplication::translate("MainWindowAMTest", "LOL:\345\210\233\345\273\272\346\210\277\351\227\264", 0));
        pushButton_lol_join->setText(QApplication::translate("MainWindowAMTest", "LOL:\345\212\240\345\205\245\346\210\277\351\227\264", 0));
        pushButton_lol_play->setText(QApplication::translate("MainWindowAMTest", "LOL:\345\274\200\345\247\213\346\270\270\346\210\217", 0));
        groupBox_HS->setTitle(QApplication::translate("MainWindowAMTest", "\347\202\211\347\237\263\344\274\240\350\257\264", 0));
        pushButton_hs_reqFrient->setText(QApplication::translate("MainWindowAMTest", "HS:\346\267\273\345\212\240\345\245\275\345\217\213", 0));
        pushButton_hs_reqChallenge->setText(QApplication::translate("MainWindowAMTest", "HS:\345\217\221\350\265\267\346\214\221\346\210\230", 0));
        pushButton_hs_acceptChallenge->setText(QApplication::translate("MainWindowAMTest", "HS\357\274\232\346\216\245\346\224\266\346\214\221\346\210\230", 0));
        pushButton_hs_acceptFriend->setText(QApplication::translate("MainWindowAMTest", "HS:\346\216\245\345\217\227\345\245\275\345\217\213", 0));
        pushButton_hs_injectDll->setText(QApplication::translate("MainWindowAMTest", "HS:\346\263\250\345\205\245DLL", 0));
        pushButton_hs_msg_2005->setText(QApplication::translate("MainWindowAMTest", "HS:MSG_2005", 0));
        pushButton_hs_msg_2006->setText(QApplication::translate("MainWindowAMTest", "HS:MSG_2006", 0));
        pushButton_hs_pipei->setText(QApplication::translate("MainWindowAMTest", "HS:\345\214\271\351\205\215\346\210\220\345\212\237", 0));
        pushButton_hs_end->setText(QApplication::translate("MainWindowAMTest", "HS:\346\270\270\346\210\217\347\273\223\346\235\237", 0));
        pushButton_test->setText(QApplication::translate("MainWindowAMTest", "\346\265\213  \350\257\225", 0));
        label->setText(QApplication::translate("MainWindowAMTest", "\345\217\202\346\225\2601", 0));
        label_2->setText(QApplication::translate("MainWindowAMTest", "\345\217\202\346\225\2602", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindowAMTest: public Ui_MainWindowAMTest {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AMTEST_H
