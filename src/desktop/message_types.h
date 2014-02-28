#ifndef MESSAGE_TYPES_H
#define MESSAGE_TYPES_H

#include <QString>
#include <QVector>
#include <QMap>


namespace LifeMotif
{

typedef quint64 ThrIdType;
typedef quint64 MsgIdType;
typedef QString DateType;

struct GoogleMessageGroup
{
  ThrIdType          threadId;
  QVector<MsgIdType> messageIds;
};

typedef QMap<DateType, GoogleMessageGroup> GoogleLocalStructureType;

}

#endif // MESSAGE_TYPES_H
