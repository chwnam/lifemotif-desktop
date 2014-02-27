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

void MimeStructureDialog::on_closeButton_clicked()
{
  close();
}

QTreeWidgetItem*
  MimeStructureDialog::CreateNewItem(
    const QString& field,
    const QString& value)
{
  QTreeWidgetItem *pItem = new QTreeWidgetItem();
  pItem->setText(fieldColumn, field);
  pItem->setText(valueColumn, value);
  return pItem;
}

void
  MimeStructureDialog::Parse(const std::string& rawMessage)
{
  ui->mimeStructureTree->clear();
  mimetic::MimeEntity root(rawMessage.begin(), rawMessage.end());

  QTreeWidgetItem *pRoot = CreateNewItem(QString("Root Entity"));
  ParseMimeEntity(root, pRoot);
  ui->mimeStructureTree->insertTopLevelItem(0, pRoot);
  ui->mimeStructureTree->expandAll();
}

void
  MimeStructureDialog::ParseMimeEntity(
    const mimetic::MimeEntity& entity,
    QTreeWidgetItem *parent)
{
  QTreeWidgetItem *pHeader = CreateNewItem(QString("Header"));
  QTreeWidgetItem *pBody = CreateNewItem(QString("Body"));

  const mimetic::Header& header = entity.header();
  const mimetic::Body& body = entity.body();

  parent->addChild(pHeader);
  parent->addChild(pBody);

  ParseHeader(header, pHeader);
  ParseBody(body, pBody);
}

void
  MimeStructureDialog::ParseHeader(
    const mimetic::Header& header,
    QTreeWidgetItem* parent)
{
  mimetic::Header::const_iterator it;
  for (it = header.begin(); it != header.end(); ++it) {

    QTreeWidgetItem *pItem
        = CreateNewItem(
            QString(it->name().c_str()),
            QString(it->value().c_str()));
    parent->addChild(pItem);
  }
}

void
  MimeStructureDialog::ParseBody(
    const mimetic::Body& body,
    QTreeWidgetItem* parent)
{

  QTreeWidgetItem* pItem;

  if (body.preamble() != "") {
    pItem = CreateNewItem(
              QString("Preamble"),
              QString(body.preamble().c_str()));

    parent->addChild(pItem);
  }

  if (body.empty() == false) {

    if (body.size() > 60) {
      std::string stringfix = body.substr(0, 60);
      stringfix += " <skipped>";
      pItem = CreateNewItem(
                QString("raw content"),
                QString(stringfix.c_str()));
    } else {
      pItem = CreateNewItem(
                QString("raw content"),
                QString(body.c_str()));
    }
    parent->addChild(pItem);
  }

  const mimetic::MimeEntityList &parts = body.parts();
  mimetic::MimeEntityList::const_iterator it;
  for(it = parts.begin(); it != parts.end(); ++it) {
    pItem = CreateNewItem(QString("Entity"));
    ParseMimeEntity(**it, pItem);
    parent->addChild(pItem);
  }

  if (body.epilogue() != "") {
    pItem = CreateNewItem(
              QString("Epilogue"),
              QString(body.epilogue().c_str()));

    parent->addChild(pItem);
  }
}
