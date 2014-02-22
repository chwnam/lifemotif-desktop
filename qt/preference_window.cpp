#include "preference_window.h"
#include "ui_preference_window.h"
#include "lifemotif_settings.h"

#include <QMessageBox>
#include <QDir>
#include <QFile>
#include <QDebug>

PreferenceWindow::PreferenceWindow(
    QWidget *parent) :
  QDialog(parent),
  ui(new Ui::PreferenceWindow)
{
  ui->setupUi(this);
}

PreferenceWindow::~PreferenceWindow()
{
  delete ui;
}

void PreferenceWindow::UpdateUI()
{
}

void PreferenceWindow::ReadFromSettings()
{
  QString pythonConfig = LifeMotifSettings::PythonConfig();
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
  QFileInfoList entries
      = QDir(LifeMotifSettings::CacheDir())
          .entryInfoList(QDir::Files);

  qDebug() << "Removing all" << entries.size() << "entrie(s):";
  QFile f;

  for(QFileInfoList::iterator it = entries.begin(); it != entries.end(); ++it) {
    QString pathToRemove = it->filePath();

    qDebug() << pathToRemove.toStdString().c_str();
    f.setFileName(pathToRemove);
    if (f.remove() == false) {
      qDebug() << "[!!]" << pathToRemove << "failed to remove!";
    }
  }
}
