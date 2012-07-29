/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Sat Jul 28 21:41:54 2012
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QScrollArea>
#include <QtGui/QSlider>
#include <QtGui/QStatusBar>
#include <QtGui/QTextBrowser>
#include <QtGui/QTextEdit>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QSlider *J;
    QSlider *QN1;
    QSlider *QN2;
    QSlider *horizontalSlider_4;
    QSlider *duration;
    QTextEdit *filename;
    QPushButton *go;
    QPushButton *stop;
    QPushButton *reset;
    QRadioButton *lattice;
    QRadioButton *orderparam;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *label_6;
    QLabel *label_7;
    QLabel *label_8;
    QLabel *label_9;
    QScrollArea *dialogueArea;
    QWidget *scrollAreaWidgetContents;
    QTextBrowser *dialogueText;
    QTextBrowser *MainDisplay;
    QLabel *labelJ;
    QLabel *labelQN1;
    QLabel *labelQN2;
    QLabel *labelQuestion;
    QMenuBar *menuBar;
    QMenu *menuThe_Model_with_no_name;
    QMenu *menuJhaberstroh_lbl_gov_whitelam_lbl_gov;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(482, 454);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        J = new QSlider(centralWidget);
        J->setObjectName(QString::fromUtf8("J"));
        J->setGeometry(QRect(320, 170, 111, 16));
        J->setMaximum(5000);
        J->setOrientation(Qt::Horizontal);
        QN1 = new QSlider(centralWidget);
        QN1->setObjectName(QString::fromUtf8("QN1"));
        QN1->setGeometry(QRect(320, 190, 111, 16));
        QN1->setOrientation(Qt::Horizontal);
        QN2 = new QSlider(centralWidget);
        QN2->setObjectName(QString::fromUtf8("QN2"));
        QN2->setGeometry(QRect(320, 210, 111, 16));
        QN2->setOrientation(Qt::Horizontal);
        horizontalSlider_4 = new QSlider(centralWidget);
        horizontalSlider_4->setObjectName(QString::fromUtf8("horizontalSlider_4"));
        horizontalSlider_4->setGeometry(QRect(320, 230, 111, 16));
        horizontalSlider_4->setOrientation(Qt::Horizontal);
        duration = new QSlider(centralWidget);
        duration->setObjectName(QString::fromUtf8("duration"));
        duration->setGeometry(QRect(10, 180, 19, 160));
        duration->setOrientation(Qt::Vertical);
        filename = new QTextEdit(centralWidget);
        filename->setObjectName(QString::fromUtf8("filename"));
        filename->setGeometry(QRect(300, 260, 171, 31));
        go = new QPushButton(centralWidget);
        go->setObjectName(QString::fromUtf8("go"));
        go->setGeometry(QRect(350, 320, 87, 27));
        go->setDefault(false);
        stop = new QPushButton(centralWidget);
        stop->setObjectName(QString::fromUtf8("stop"));
        stop->setGeometry(QRect(350, 360, 87, 27));
        reset = new QPushButton(centralWidget);
        reset->setObjectName(QString::fromUtf8("reset"));
        reset->setGeometry(QRect(130, 90, 87, 27));
        lattice = new QRadioButton(centralWidget);
        lattice->setObjectName(QString::fromUtf8("lattice"));
        lattice->setGeometry(QRect(80, 120, 71, 16));
        orderparam = new QRadioButton(centralWidget);
        orderparam->setObjectName(QString::fromUtf8("orderparam"));
        orderparam->setGeometry(QRect(180, 120, 121, 16));
        label = new QLabel(centralWidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(320, 50, 21, 20));
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(320, 80, 21, 20));
        label_3 = new QLabel(centralWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(390, 60, 31, 20));
        label_4 = new QLabel(centralWidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(410, 30, 21, 20));
        label_5 = new QLabel(centralWidget);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(410, 90, 16, 20));
        label_6 = new QLabel(centralWidget);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(300, 170, 21, 20));
        label_7 = new QLabel(centralWidget);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(300, 190, 21, 20));
        label_8 = new QLabel(centralWidget);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(300, 210, 21, 20));
        label_9 = new QLabel(centralWidget);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(300, 230, 21, 20));
        dialogueArea = new QScrollArea(centralWidget);
        dialogueArea->setObjectName(QString::fromUtf8("dialogueArea"));
        dialogueArea->setGeometry(QRect(40, 10, 251, 71));
        dialogueArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 249, 69));
        dialogueText = new QTextBrowser(scrollAreaWidgetContents);
        dialogueText->setObjectName(QString::fromUtf8("dialogueText"));
        dialogueText->setGeometry(QRect(0, 0, 251, 71));
        dialogueText->setMaximumSize(QSize(256, 192));
        dialogueArea->setWidget(scrollAreaWidgetContents);
        MainDisplay = new QTextBrowser(centralWidget);
        MainDisplay->setObjectName(QString::fromUtf8("MainDisplay"));
        MainDisplay->setGeometry(QRect(40, 170, 251, 192));
        MainDisplay->setMaximumSize(QSize(256, 192));
        labelJ = new QLabel(centralWidget);
        labelJ->setObjectName(QString::fromUtf8("labelJ"));
        labelJ->setGeometry(QRect(440, 170, 58, 17));
        labelQN1 = new QLabel(centralWidget);
        labelQN1->setObjectName(QString::fromUtf8("labelQN1"));
        labelQN1->setGeometry(QRect(440, 190, 58, 17));
        labelQN2 = new QLabel(centralWidget);
        labelQN2->setObjectName(QString::fromUtf8("labelQN2"));
        labelQN2->setGeometry(QRect(440, 210, 58, 17));
        labelQuestion = new QLabel(centralWidget);
        labelQuestion->setObjectName(QString::fromUtf8("labelQuestion"));
        labelQuestion->setGeometry(QRect(440, 230, 58, 17));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 482, 25));
        menuThe_Model_with_no_name = new QMenu(menuBar);
        menuThe_Model_with_no_name->setObjectName(QString::fromUtf8("menuThe_Model_with_no_name"));
        menuJhaberstroh_lbl_gov_whitelam_lbl_gov = new QMenu(menuBar);
        menuJhaberstroh_lbl_gov_whitelam_lbl_gov->setObjectName(QString::fromUtf8("menuJhaberstroh_lbl_gov_whitelam_lbl_gov"));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menuThe_Model_with_no_name->menuAction());
        menuBar->addAction(menuJhaberstroh_lbl_gov_whitelam_lbl_gov->menuAction());

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
        go->setText(QApplication::translate("MainWindow", "Go", 0, QApplication::UnicodeUTF8));
        stop->setText(QApplication::translate("MainWindow", "Stop", 0, QApplication::UnicodeUTF8));
        reset->setText(QApplication::translate("MainWindow", "Reset", 0, QApplication::UnicodeUTF8));
        lattice->setText(QApplication::translate("MainWindow", "Lattice", 0, QApplication::UnicodeUTF8));
        orderparam->setText(QApplication::translate("MainWindow", "Order Params", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("MainWindow", "N1:", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("MainWindow", "N2:", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("MainWindow", "pdel:", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("MainWindow", "R:", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("MainWindow", "T:", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("MainWindow", "J:", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("MainWindow", "Q1:", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("MainWindow", "Q2:", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("MainWindow", "???", 0, QApplication::UnicodeUTF8));
        labelJ->setText(QString());
        labelQN1->setText(QString());
        labelQN2->setText(QString());
        labelQuestion->setText(QString());
        menuThe_Model_with_no_name->setTitle(QApplication::translate("MainWindow", "The Model with no name", 0, QApplication::UnicodeUTF8));
        menuJhaberstroh_lbl_gov_whitelam_lbl_gov->setTitle(QApplication::translate("MainWindow", "Molecular Foundry, LBNL", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
