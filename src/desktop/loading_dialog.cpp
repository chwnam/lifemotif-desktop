//#include "loading_dialog.h"
//#include "ui_loading_dialog.h"
//#include <QDebug>
//#include <QThread>
//#include <QThreadPool>

//const QString LoadingDialog::loadingMessages[4] = {
//  "Loading",
//  "Loading.",
//  "Loading..",
//  "Loading..."
//};

//LoadingDialog::LoadingDialog(QWidget *parent) :
//  QDialog(parent),
//  ui(new Ui::LoadingDialog)
//{
//  ui->setupUi(this);
//  SetupFrame();
//}

//LoadingDialog::~LoadingDialog()
//{
//  delete ui;
//}

//void LoadingDialog::SetupFrame()
//{
//  setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
//}

//void LoadingDialog::CenterPosition()
//{
//  move(
//  parentWidget()->window()->frameGeometry().topLeft() +
//  parentWidget()->window()->rect().center() -
//  rect().center());
//}

//void LoadingDialog::DisplayLoadingMessage()
//{
//  loopOver = true;
//  for(int i = 0; loopOver; ++i) {
//    ui->loadingLabel->setText(loadingMessages[i%4]);
//    QThread::msleep(850);
//  }
//  //qDebug() << "Exiting DisplayLoadingMessage()...";
//}

//void LoadingDialog::WaitAndClose()
//{
//  loopOver = false;
//  //qDebug() << "loopOver is false";
//}

//void LoadingDialog::ReadyForDisplay()
//{
//  if (RunnableHelper<LoadingDialog>
//      *openTask = new RunnableHelper<LoadingDialog>(
//        this, &LoadingDialog::DisplayLoadingMessage)) {
//        QThreadPool::globalInstance()->start(openTask);
//  }
//}

//void LoadingDialog::ReadyForClose()
//{
//  if (RunnableHelper<LoadingDialog>
//      *closeTask = new RunnableHelper<LoadingDialog>(
//        this, &LoadingDialog::WaitAndClose)) {
//        QThreadPool::globalInstance()->start(closeTask);
//  }
//}

//void LoadingDialog::showEvent(QShowEvent *event)
//{
//  CenterPosition();
//  ReadyForDisplay();
//  QDialog::showEvent(event);
//}

//void LoadingDialog::closeEvent(QCloseEvent *event)
//{
//  ReadyForClose();
//  QDialog::closeEvent(event);
//}

//void LoadingDialog::on_pushButton_clicked()
//{
//   close();
//}
