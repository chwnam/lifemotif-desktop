#ifndef MIME_RAW_MESSAGE_DIALOG_H
#define MIME_RAW_MESSAGE_DIALOG_H

#include <QDialog>

namespace Ui {
class MimeRawMessageDialog;
}

class MimeRawMessageDialog : public QDialog
{
  Q_OBJECT

public:
  explicit MimeRawMessageDialog(QWidget *parent = 0);
  ~MimeRawMessageDialog();

  void SetEditText(const QString& text);

private slots:
  void on_closeButton_clicked();

private:
  Ui::MimeRawMessageDialog *ui;
};

#endif // MIME_RAW_MESSAGE_DIALOG_H
