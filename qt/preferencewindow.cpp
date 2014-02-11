#include "preferencewindow.h"
#include "ui_preferencewindow.h"
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

  // python_config
  ui->scriptPathEdit->setText(pythonConfig);
}

void PreferenceWindow::on_refreshSettingButton_clicked()
{
    ReadFromSettings();
}
