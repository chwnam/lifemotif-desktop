#include "tag_reset_dialog.h"
#include "ui_tag_reset_dialog.h"

TagResetDialog::TagResetDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::TagResetDialog)
{
  ui->setupUi(this);
}

TagResetDialog::~TagResetDialog()
{
  delete ui;
}

char TagResetDialog::NewPrefix()
{
  return ui->prefix_edit->text()[0].toUpper().toLatin1();
}

unsigned int TagResetDialog::NewNum()
{
  return ui->num_edit->text().toUInt();
}
