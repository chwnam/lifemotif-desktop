#include "preference_window.h"
#include "ui_preference_window.h"

#include <QMessageBox>
#include <QDir>
#include <QFile>
#include <QDebug>

#include "settings.h"

PreferenceWindow::PreferenceWindow(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::PreferenceWindow)
{
  ui->setupUi(this);

  GetMailboxList();
  UpdateMailboxComboBox();
}

PreferenceWindow::~PreferenceWindow()
{
  delete ui;
}

void PreferenceWindow::UpdateUI()
{
}

void PreferenceWindow::on_refreshSettingButton_clicked()
{
}

void PreferenceWindow::on_cleanDiskCacheButton_clicked()
{
  const QString title("Clean cache");
  const QString text("Wipe all cache entries! Are you sure?");

  if (QMessageBox::question(this, title, text) == QMessageBox::Yes) {
    WipeCacheEntries();
  }
}

void PreferenceWindow::WipeCacheEntries()
{
//  QFileInfoList entries
//      = QDir(Settings::CacheDir())
//          .entryInfoList(QDir::Files);

//  qDebug() << "Removing all" << entries.size() << "entrie(s):";
//  QFile f;

//  for(QFileInfoList::iterator it = entries.begin(); it != entries.end(); ++it) {
//    QString pathToRemove = it->filePath();

//    qDebug() << pathToRemove.toStdString().c_str();
//    f.setFileName(pathToRemove);
//    if (f.remove() == false) {
//      qDebug() << "[!!]" << pathToRemove << "failed to remove!";
//    }
//  }
}

void PreferenceWindow::GetMailboxList()
{
//  if (imap) {
//    bp::object pythonMailboxList = imap->GetGoogleMailboxSimpleList();
//    MailboxListExtract(pythonMailboxList, mailboxList);
//  }
}

void PreferenceWindow::UpdateMailboxComboBox()
{
  ui->mailboxComboBox->clear();
  ui->mailboxComboBox->addItems(mailboxList);
  //for(std::size_t i = 0; i < mailboxList.size(); ++i) {
  //  ui->mailboxComboBox->addItem(mailboxList[i]);
  //}
}
