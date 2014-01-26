#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_activateButton_clicked()
{
    QMessageBox::information(this, "Hello!", "Activated!", QMessageBox::Ok);
}

void MainWindow::on_authenticateButton_clicked()
{
    QMessageBox::information(this, "Authenticate", "Authentication", QMessageBox::Ok);
}
