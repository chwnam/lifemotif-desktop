#include "mime_structure_dialog.h"
#include "ui_mime_structure_dialog.h"

MimeStructureDialog::MimeStructureDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::MimeStructureDialog)
{
  ui->setupUi(this);
}

MimeStructureDialog::~MimeStructureDialog()
{
  delete ui;
}
