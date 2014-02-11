#ifndef MESSAGE_TYPES_H
#define MESSAGE_TYPES_H

#include <string>
#include <vector>
#include <map>

typedef unsigned long long ThrIdType;
typedef unsigned long long MsgIdType;
typedef std::string DateType;

struct MessageGroup
{
  ThrIdType threadId;
  std::vector<MsgIdType> messageIds;
};

typedef std::map<DateType, MessageGroup> LocalStructureType;

#endif // MESSAGE_TYPES_H
