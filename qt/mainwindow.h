#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <boost/shared_ptr.hpp>
#include <QMainWindow>

#include "./python_wrapper/python_wrapper.h"
#include "localstructure_extract.h"
#include "email_cache.h"

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

     void on_calendarWidget_clicked(const QDate &date);

     void on_diaryList_clicked(const QModelIndex &index);

     void on_actionBuild_Local_Structure_triggered();

private:
    void InitWrappers();
    void LoadLocalStructure();
    void UpdateCalendar();

private:
    Ui::MainWindow *ui;

    // wrappers
    boost::shared_ptr<GoogleOauth2Wrapper> oauth2Wrapper;
    boost::shared_ptr<GoogleImapWrapper>   imapWrapper;

    // local structure
    LocalStructureType localStructure;

    boost::shared_ptr<EmailCache> emailCache;
};

#endif // MAINWINDOW_H
