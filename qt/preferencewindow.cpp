#include "preferencewindow.h"
#include "ui_preferencewindow.h"
#include "settingscontrol.h"

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
  QSettings& settings = SettingsControl::GetSettings();

  // python_config
  ui->scriptPathEdit->setText(settings.value("python_config").toString());
}

void PreferenceWindow::on_refreshSettingButton_clicked()
{
    ReadFromSettings();
}
