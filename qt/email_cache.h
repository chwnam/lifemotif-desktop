#ifndef EMAIL_CACHE_H
#define EMAIL_CACHE_H

#include "message_types.h"
#include <set>
#include <QString>

class EmailCache
{
public:
  EmailCache(const QString& _cacheDir);

  bool HasCache(MsgIdType messageId) const;
  std::string GetCache(MsgIdType messageId) const;
  void SetCache(MsgIdType messageId, const std::string message);

private:
  void BuildCacheIndex();

  QString cacheDir;
  std::set<MsgIdType> cacheIndex;
};

#endif // EMAIL_CACHE_H
