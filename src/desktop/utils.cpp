#include "lifemotif_utils.h"

#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>

#include "lifemotif_config.h"
#include "lifemotif_settings.h"

GoogleOauth2Wrapper* LifeMotifUtils::CreateOauth2Wrapper()
{
  GoogleOauth2Wrapper* ptr
      = new GoogleOauth2Wrapper(
        LIFEMOTIF_GOOGLE_OAUTH2_WRAPPER_MODULE,
        LIFEMOTIF_GOOGLE_OAUTH2_WRAPPER_CLASS);
  return ptr;
}

LocalStructureWrapper* LifeMotifUtils::CreateLocalStructureWrapper()
{
  LocalStructureWrapper* ptr
    = new LocalStructureWrapper(
        LIFEMOTIF_LOCAL_STRUCTURE_WRAPPER_MODULE,
        LIFEMOTIF_LOCAL_STRUCTURE_WRAPPER_CLASS);
  return ptr;
}

QString LifeMotifUtils::Strip(const QString& input, const QChar& s)
{
  int b, e;

  b = input.startsWith(s) ? 1 : 0;
  e = input.endsWith(s)   ? input.size()-1 : input.size();

  return input.mid(b, e - b);
}

bool
  LifeMotifUtils::SaveJson(
    const QString& fileName, const QVariantMap& map)
{
  const QByteArray array
    = QJsonDocument( QJsonObject::fromVariantMap(map) ).toJson();
  QFile w(fileName);
  if (w.open(QIODevice::WriteOnly|QIODevice::Text)) {
    w.write(array);
    w.close();
    return true;
  }
  return false;
}

QVariant
  LifeMotifUtils::LoadJson(
    const QString& fileName, bool* isQVariantMap)
{
  QVariant v;
  QFile    r(fileName);
  if (r.open(QIODevice::ReadOnly|QIODevice::Text)) {
    QJsonDocument doc = QJsonDocument::fromJson(r.readAll());
    *isQVariantMap = doc.isObject();
    v = doc.toVariant();
  }
  return v;
}
