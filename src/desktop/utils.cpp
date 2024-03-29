#include "utils.h"

#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>

#include "config.h"
#include "google_oauth2.h"
#include "settings.h"


namespace LifeMotif {

QString Utils::Strip(const QString& input, const QChar& s)
{
  int b, e;

  b = input.startsWith(s) ? 1 : 0;
  e = input.endsWith(s)   ? input.size()-1 : input.size();

  return input.mid(b, e - b);
}

bool
Utils::SaveJson(
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
Utils::LoadJson(
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

bool
Utils::WaitForSignal(
  QObject* sender, const char *signal, int timeoutMsec)
{
  QEventLoop loop;
  QTimer     timer;

  timer.setInterval(timeoutMsec);
  timer.setSingleShot(true);

  loop.connect(sender, signal, SLOT(quit()));
  loop.connect(&timer, SIGNAL(timeout()), SLOT(quit()));

  timer.start();
  loop.exec();

  return timer.isActive();
}

}
