/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCalendarWidget>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QPlainTextEdit *plainTextEdit;
    QCalendarWidget *calendarWidget;
    QLineEdit *lineEdit;
    QLabel *mailBoxLabel;
    QListWidget *dirayList;
    QLabel *diaryListLabel;
    QPushButton *activateButton;
    QPushButton *authenticateButton;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(730, 536);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        plainTextEdit = new QPlainTextEdit(centralWidget);
        plainTextEdit->setObjectName(QStringLiteral("plainTextEdit"));
        plainTextEdit->setGeometry(QRect(240, 10, 481, 481));
        calendarWidget = new QCalendarWidget(centralWidget);
        calendarWidget->setObjectName(QStringLiteral("calendarWidget"));
        calendarWidget->setGeometry(QRect(10, 10, 221, 166));
        lineEdit = new QLineEdit(centralWidget);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));
        lineEdit->setGeometry(QRect(80, 190, 151, 21));
        mailBoxLabel = new QLabel(centralWidget);
        mailBoxLabel->setObjectName(QStringLiteral("mailBoxLabel"));
        mailBoxLabel->setGeometry(QRect(10, 190, 62, 16));
        dirayList = new QListWidget(centralWidget);
        dirayList->setObjectName(QStringLiteral("dirayList"));
        dirayList->setGeometry(QRect(10, 240, 221, 141));
        diaryListLabel = new QLabel(centralWidget);
        diaryListLabel->setObjectName(QStringLiteral("diaryListLabel"));
        diaryListLabel->setGeometry(QRect(10, 220, 62, 16));
        activateButton = new QPushButton(centralWidget);
        activateButton->setObjectName(QStringLiteral("activateButton"));
        activateButton->setGeometry(QRect(10, 460, 221, 32));
        authenticateButton = new QPushButton(centralWidget);
        authenticateButton->setObjectName(QStringLiteral("authenticateButton"));
        authenticateButton->setGeometry(QRect(10, 390, 221, 32));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 730, 22));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        lineEdit->setText(QApplication::translate("MainWindow", "\352\260\234\354\235\270/\354\235\274\352\270\260", 0));
        mailBoxLabel->setText(QApplication::translate("MainWindow", "\355\216\270\354\247\200\355\225\250:", 0));
        diaryListLabel->setText(QApplication::translate("MainWindow", "\354\235\274\352\270\260 \353\252\251\353\241\235", 0));
        activateButton->setText(QApplication::translate("MainWindow", "\355\224\204\353\241\234\352\267\270\353\236\250 \353\217\231\354\236\221", 0));
        authenticateButton->setText(QApplication::translate("MainWindow", "\355\201\264\353\235\274\354\235\264\354\226\270\355\212\270 \354\235\270\354\246\235", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
