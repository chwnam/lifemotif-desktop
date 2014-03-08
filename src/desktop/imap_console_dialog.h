#ifndef IMAP_CONSOLE_DIALOG_H
#define IMAP_CONSOLE_DIALOG_H

#include <QDialog>

#include "imap_manager.h"


namespace Ui {
class ImapConsoleDialog;
}

class ImapConsoleDialog : public QDialog
{
  Q_OBJECT

public:
  explicit ImapConsoleDialog(
      LifeMotif::ImapManager *_imapManager, QWidget *parent = 0);
  ~ImapConsoleDialog();

private slots:
  void on_submitButton_clicked();
  void on_commandEdit_returnPressed();

  // when imap_manager signals dataReceived
  void ImapDataReceived(const QByteArray&);

  // when imap_manager signals clientRequest
  void ImapClientRequest(const QByteArray&);

private:
  void updateUI();
  void submit();
  void appendString(const QString& string);

private:
  Ui::ImapConsoleDialog *ui;
  LifeMotif::ImapManager *imapManager;
};

#endif // IMAP_CONSOLE_DIALOG_H
