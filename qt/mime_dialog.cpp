#include "mime_dialog.h"
#include "ui_mime_dialog.h"

MimeDialog::MimeDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::MimeDialog)
{
  ui->setupUi(this);
}

MimeDialog::~MimeDialog()
{
  delete ui;
}

void MimeDialog::SetEditText(const QString& text)
{
  ui->rawMimeText->setPlainText(text);
}

void MimeDialog::on_closeButton_clicked()
{
  close();
}
