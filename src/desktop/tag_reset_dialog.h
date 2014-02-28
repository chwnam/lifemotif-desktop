#ifndef TAG_RESET_DIALOG_H
#define TAG_RESET_DIALOG_H

#include <QDialog>

namespace Ui {
class TagResetDialog;
}

class TagResetDialog : public QDialog
{
  Q_OBJECT

public:
  explicit TagResetDialog(QWidget *parent = 0);
  ~TagResetDialog();

  char NewPrefix();
  unsigned int NewNum();

private:
  Ui::TagResetDialog *ui;
};

#endif // TAG_RESET_DIALOG_H
