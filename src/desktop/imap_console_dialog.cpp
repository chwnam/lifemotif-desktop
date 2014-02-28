#include "imap_console_dialog.h"
#include "ui_imap_console_dialog.h"

#include <QDebug>
#include <QString>
#include <QScrollBar>

ImapConsoleDialog::ImapConsoleDialog(QWidget *parent) :
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
  if (text.size()) {
    //const QString command
//        = QString::fromStdString(tag.issue()) + QChar(' ') + text;

//    appendString(command);
    //socket->write(command.toUtf8() + "\r\n");
    //tag.advance();
//  ui->commandEdit->clear();
    updateUI();
  }
}

void ImapConsoleDialog::on_commandEdit_returnPressed()
{
  submit();
}

void ImapConsoleDialog::updateUI()
{
  //ui->tagEdit->setText(QString::fromStdString(tag.issue()));
}
