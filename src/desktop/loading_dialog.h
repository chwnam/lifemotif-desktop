#ifndef LOADING_DIALOG_H
#define LOADING_DIALOG_H

#include <QDialog>
#include <QString>
#include "runnable_helper.h"

namespace Ui {
class LoadingDialog;
}

class LoadingDialog : public QDialog
{
  Q_OBJECT

public:
  explicit LoadingDialog(QWidget *parent = 0);
  ~LoadingDialog();

private slots:
  void on_pushButton_clicked();

private:
  void showEvent(QShowEvent *);
  void closeEvent(QCloseEvent *);
  void SetupFrame();
  void CenterPosition();
  void DisplayLoadingMessage();
  void ReadyForDisplay();
  void ReadyForClose();
  void WaitAndClose();

private:
  Ui::LoadingDialog     *ui;
  static const QString  loadingMessages[4];
  bool                  loopOver;

  friend class RunnableHelperClass;
};

#endif // LOADING_DIALOG_H
