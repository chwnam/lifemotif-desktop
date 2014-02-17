#include "mime_raw_message_dialog.h"
#include "ui_mime_raw_message_dialog.h"

MimeRawMessageDialog::MimeRawMessageDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::MimeRawMessageDialog)
{
  ui->setupUi(this);
}

MimeRawMessageDialog::~MimeRawMessageDialog()
{
  delete ui;
}

void MimeRawMessageDialog::SetEditText(const QString& text)
{
  ui->rawMimeText->setPlainText(text);
}

void MimeRawMessageDialog::on_closeButton_clicked()
{
  close();
}
