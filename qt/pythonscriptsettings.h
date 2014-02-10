#ifndef PYTHONSCRIPTSETTINGS_H
#define PYTHONSCRIPTSETTINGS_H


#include <QVariant>

class PythonScriptSettings
{
private:
  PythonScriptSettings();
  ~PythonScriptSettings();

  static QVariantMap *instance;

public:
  static QVariantMap& GetSettings();
  static void Init(const QString& path);
};

#endif // PYTHONSCRIPTSETTINGS_H
