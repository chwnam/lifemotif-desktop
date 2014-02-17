#ifndef MIME_STRUCTURE_DIALOG_H
#define MIME_STRUCTURE_DIALOG_H

#include <QDialog>

namespace Ui {
class MimeStructureDialog;
}

class MimeStructureDialog : public QDialog
{
  Q_OBJECT

public:
  explicit MimeStructureDialog(QWidget *parent = 0);
  ~MimeStructureDialog();

private:
  Ui::MimeStructureDialog *ui;
};

#endif // MIME_STRUCTURE_DIALOG_H
