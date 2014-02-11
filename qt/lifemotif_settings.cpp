#include "lifemotif_settings.h"

#include <QFile>
#include <QJsonDocument>

#include "lifemotif_config.h"
#include "lifemotif_exceptions.h"

QSettingsPtr LifeMotifSettings::iniInstance = QSettingsPtr();
QVariantMapPtr LifeMotifSettings::jsonInstance = QVariantMapPtr();

LifeMotifSettings::LifeMotifSettings()
{
}

LifeMotifSettings::~LifeMotifSettings()
{
}

void LifeMotifSettings::Init(const QString& path)
{
  if (iniInstance == NULL) {
    iniInstance = QSettingsPtr(new QSettings(path, QSettings::IniFormat));
    if (iniInstance == NULL) throw std::bad_alloc();
  }

  if (jsonInstance == NULL) {
    QVariantMap map = LoadJson(PythonConfig());
    jsonInstance = QVariantMapPtr(new QVariantMap(map));
    if (jsonInstance == NULL) throw std::bad_alloc();
  }
}

QVariantMap LifeMotifSettings::LoadJson(const QString& path)
{
  QFile file(path);
  if (file.exists() == false) {
    throw FileNotFoundException(path.toStdString().c_str());
  }

  file.open(QIODevice::ReadOnly);
  QJsonDocument jsonDoc = QJsonDocument::fromJson(file.readAll());

  if (jsonDoc.isNull()) {
    throw FileInvalidException(path.toStdString().c_str());
  }
  return jsonDoc.toVariant().toMap();
}
