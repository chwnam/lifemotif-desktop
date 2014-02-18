#include "preference_window.h"
#include "ui_preference_window.h"
#include "lifemotif_settings.h"

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
