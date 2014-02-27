#include "connect_dialog.h"
#include "ui_connect_dialog.h"

ConnectDialog::ConnectDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::ConnectDialog)
{
  ui->setupUi(this);
}

ConnectDialog::~ConnectDialog()
{
  delete ui;
}

const QString& ConnectDialog::host() const
{
  return ui->host_edit->text();
}

quint16  ConnectDialog::port() const
{
  return ui->port_edit->text().toShort();
}

bool ConnectDialog::use_ssl() const
{
  return ui->ssl_check->checkState() == Qt::Checked;
}

