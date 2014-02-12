#ifndef EMAIL_CACHE_H
#define EMAIL_CACHE_H

#include "message_types.h"
#include <set>
#include <QDir>

class EmailCache
{
public:
  EmailCache(const QString& _cacheDir);

  bool HasCache(MsgIdType messageId) const;
  QString GetCache(MsgIdType messageId) const;
  void SetCache(MsgIdType messageId, const QString& message);

private:
  void BuildCacheIndex();

  QDir cacheDir;
  std::set<MsgIdType> cacheIndex;
};

#endif // EMAIL_CACHE_H
