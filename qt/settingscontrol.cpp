#include <new>
#include <QJsonDocument>
#include "settingscontrol.h"
#include "lifemotif_config.h"

QSettings* SettingsControl::instance = NULL;

SettingsControl::SettingsControl()
{
}

SettingsControl::~SettingsControl()
{
  if (instance == NULL) {
    delete instance;
    instance = NULL;
  }
}

void SettingsControl::Init(const QString& path)
{
  if (instance == NULL) {
    instance = new QSettings(path, QSettings::IniFormat);
    if (instance == NULL) throw std::bad_alloc();
  }
}

QSettings& SettingsControl::GetSettings()
{
  return *instance;
}
