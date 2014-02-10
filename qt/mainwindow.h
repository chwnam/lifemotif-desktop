#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "./python_wrapper/python_wrapper.h"
#include "localstructure.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
     void on_activateButton_clicked();

     void on_authenticateButton_clicked();

     void on_actionOptions_triggered();

     void on_listStructureButton_clicked();

     void on_calendarWidget_clicked(const QDate &date);

     void on_diaryList_clicked(const QModelIndex &index);

private:
    Ui::MainWindow *ui;
    LocalStructure *localStructure;

    // wrappers
    GoogleImapWrapper *imapWrapper;
    GoogleOauth2Wrapper *oauth2Wrapper;
};

#endif // MAINWINDOW_H
