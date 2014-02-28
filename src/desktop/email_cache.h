#ifndef EMAIL_CACHE_H
#define EMAIL_CACHE_H

#include "message_types.h"
#include <QSet>
#include <QDir>

namespace LifeMotif {
class EmailCache
{
public:
  EmailCache(const QString& _cacheDir);

  bool    HasCache(MsgIdType messageId) const;
  QString GetCache(MsgIdType messageId) const;
  void    SetCache(MsgIdType messageId, const QString& message);

private:
  void BuildCacheIndex();

  QDir cacheDir;
  QSet<MsgIdType> cacheIndex;
};
}

#endif // EMAIL_CACHE_H
