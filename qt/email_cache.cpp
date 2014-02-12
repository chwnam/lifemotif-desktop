#include "email_cache.h"
#include <QDebug>
#include <QTextStream>
#include <iostream>

EmailCache::EmailCache(const QString& _cacheDir)
  : cacheDir(_cacheDir)
{
  BuildCacheIndex();
}

bool EmailCache::HasCache(MsgIdType messageId) const
{
  return cacheIndex.find(messageId) != cacheIndex.end();
}

QString EmailCache::GetCache(MsgIdType messageId) const
{
  QString path = cacheDir.absoluteFilePath(QString::number(messageId));
  qDebug() << "get file cache" << path;

  QFile f(path);  
  f.open(QIODevice::ReadOnly|QIODevice::Text);

  QString m = QTextStream(&f).readAll();
  f.close();

  return m;
}

void EmailCache::SetCache(MsgIdType messageId, const QString& message)
{
  QString path = cacheDir.absoluteFilePath(QString::number(messageId));
  qDebug() << "set file cache" << path;

  QFile f(path);
  f.open(QIODevice::WriteOnly|QIODevice::Text);

  QTextStream(&f) << message;
  f.close();

  cacheIndex.insert(messageId);
}

void EmailCache::BuildCacheIndex()
{
  if (cacheIndex.empty() == false) {
    cacheIndex.clear();
  }

  // Assume that cacheDir is good.
  QStringList list = cacheDir.entryList(QDir::Files|QDir::NoDotAndDotDot);
  qDebug() << "current file cache:" << list.size() << " entries\n";
  for(int i = 0; i < list.size(); ++i) {
    MsgIdType messageId = list[i].toULongLong();
    qDebug() << '\t' << messageId;
    cacheIndex.insert(messageId);
  }
}
