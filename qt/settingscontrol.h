#ifndef SETTINGSCONTROL_H
#define SETTINGSCONTROL_H

#include <QSettings>

class SettingsControl
{
private:
  SettingsControl();
  ~SettingsControl();

  static QSettings *instance;

public:
  static QSettings& GetSettings();
  static void Init(const QString& path);
};

#endif // SETTINGSCONTROL_H
