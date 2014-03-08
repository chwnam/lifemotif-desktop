#include "imap_console_dialog.h"
#include "ui_imap_console_dialog.h"

#include <QDebug>
#include <QString>
#include <QScrollBar>

ImapConsoleDialog::ImapConsoleDialog(
    LifeMotif::ImapManager *_imapManager, QWidget *parent) :
  imapManager(_imapManager),
  QDialog(parent),
  ui(new Ui::ImapConsoleDialog)
{
  ui->setupUi(this);
  updateUI();
}

ImapConsoleDialog::~ImapConsoleDialog()
{
  delete ui;
}

void ImapConsoleDialog::appendString(const QString& string)
{
  QTextCursor cursor(ui->textEdit->textCursor());
  cursor.movePosition(QTextCursor::End);
  cursor.insertText(string);
  cursor.insertText(QChar('\n'));

  QScrollBar* bar = ui->textEdit->verticalScrollBar();
  if (bar) {
    bar->setValue(bar->maximum());
  }
}

void ImapConsoleDialog::on_submitButton_clicked()
{
  submit();
}

void ImapConsoleDialog::submit()
{  
  const QString text = ui->commandEdit->text().trimmed();
  if (imapManager) {
    imapManager->SendCommand(text.toUtf8());
    updateUI();
  }
}

void ImapConsoleDialog::on_commandEdit_returnPressed()
{
  submit();
}

void ImapConsoleDialog::updateUI()
{
  ui->commandEdit->clear();
}

// when imap_manager signals dataReceived
void ImapConsoleDialog::ImapDataReceived(const QByteArray& response)
{
  ui->textEdit->append(QString::fromUtf8(response));
}

// when imap_manager signals clientRequest
void ImapConsoleDialog::ImapClientRequest(const QByteArray& command)
{
  ui->textEdit->append(QString::fromUtf8(command));
}
