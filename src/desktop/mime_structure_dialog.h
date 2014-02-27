#ifndef MIME_STRUCTURE_DIALOG_H
#define MIME_STRUCTURE_DIALOG_H

#include <QDialog>
#include <QTreeWidgetItem>
#include <mimetic/mimetic.h>

namespace Ui {
class MimeStructureDialog;
}

class MimeStructureDialog : public QDialog
{
  Q_OBJECT

public:
  explicit MimeStructureDialog(QWidget *parent = 0);
  ~MimeStructureDialog();

  void Parse(const std::string& rawMessage);

private slots:
  void on_closeButton_clicked();

private:
  QTreeWidgetItem*
    CreateNewItem(
      const QString& field = QString(),
      const QString& value = QString());

  void
    ParseMimeEntity(
      const mimetic::MimeEntity& entity,
      QTreeWidgetItem* parent);

  void
    ParseHeader(
      const mimetic::Header& header,
      QTreeWidgetItem* parent);

  void
    ParseBody(
      const mimetic::Body& body,
      QTreeWidgetItem* parent);

private:
  enum {fieldColumn = 0, valueColumn = 1};

  Ui::MimeStructureDialog *ui;
};

#endif // MIME_STRUCTURE_DIALOG_H
