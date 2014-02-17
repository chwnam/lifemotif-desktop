#ifndef MIME_DIALOG_H
#define MIME_DIALOG_H

#include <QDialog>

namespace Ui {
class MimeDialog;
}

class MimeDialog : public QDialog
{
  Q_OBJECT

public:
  explicit MimeDialog(QWidget *parent = 0);
  ~MimeDialog();

  void SetEditText(const QString& text);

private slots:
  void on_closeButton_clicked();

private:
  Ui::MimeDialog *ui;
};

#endif // MIME_DIALOG_H
