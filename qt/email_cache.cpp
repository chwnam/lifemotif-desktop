#include "email_cache.h"
#include <QDir>
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

std::string EmailCache::GetCache(MsgIdType messageId) const
{
  QDir dir(cacheDir);
  QString s = QString::number(messageId);
  QString path = dir.absoluteFilePath(s);

  std::cout << path.toStdString() << std::endl;

  QFile f(path);
  QString m;

  f.open(QIODevice::ReadOnly|QIODevice::Text);
  QTextStream in(&f);

  while(!in.atEnd()) {
    QString line = in.readLine();
    m += line;
  }

  return m.toStdString();
}

void EmailCache::SetCache(MsgIdType messageId, const std::string message)
{
  QDir dir(cacheDir);
  QString s = QString::number(messageId);
  QString path = dir.absoluteFilePath(s);
  QString m = QString::fromStdString(message);

  QFile f(path);
  f.open(QIODevice::WriteOnly|QIODevice::Text);

  QTextStream out(&f);
  out << m;
  out.flush();
  f.close();

  cacheIndex.insert(messageId);
}

void EmailCache::BuildCacheIndex()
{
  if (cacheIndex.empty() == false) {
    cacheIndex.clear();
  }

  QDir p(cacheDir);

  if (p.exists()) {
    QStringList list = p.entryList(QDir::Files|QDir::NoDotAndDotDot);
    std::cout << list.size() << " entries\n";

    for(int i = 0; i < list.size(); ++i) {
      std::cout << list[i].toStdString() << std::endl;
      MsgIdType messageId = list[i].toULongLong();
      cacheIndex.insert(messageId);
    }
  } else {
    // warning! cache directory is not exists or is not a directory!
  }

  // debug
  std::cout << "Debug cache:\n";
  for(std::set<MsgIdType>::iterator it = cacheIndex.begin();
      it != cacheIndex.end(); ++it) {
    std::cout << (*it) << std::endl;
  }
  std::cout << "\n";
}
