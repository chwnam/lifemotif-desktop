#ifndef IMAP_CONSOLE_DIALOG_H
#define IMAP_CONSOLE_DIALOG_H

#include <QDialog>

namespace Ui {
class ImapConsoleDialog;
}

class ImapConsoleDialog : public QDialog
{
  Q_OBJECT

public:
  explicit ImapConsoleDialog(QWidget *parent = 0);
  ~ImapConsoleDialog();

private slots:
  void on_submitButton_clicked();
  void on_commandEdit_returnPressed();  

private:
  void updateUI();
  void submit();
  void appendString(const QString& string);

private:
  Ui::ImapConsoleDialog *ui;
};

#endif // IMAP_CONSOLE_DIALOG_H
