#ifndef LOCALSTRUCTURE_H
#define LOCALSTRUCTURE_H

#include <boost/python.hpp>
#include <string>
#include <vector>
#include <map>

namespace bp = boost::python;

typedef unsigned long long ThrIdType;
typedef unsigned long long MsgIdType;
typedef std::string DateType;

struct MessageGroup
{
  ThrIdType threadId;
  std::vector<MsgIdType> messageIds;
};

// key: date
// val: (thread_id, [message_ids])
typedef std::map<DateType, MessageGroup> LocalStructureType;

// function class
class LocalStructureExtract
{
public:
  LocalStructureExtract(bp::object& inObject, LocalStructureType& outStructure);
  static void PrintStructure(const LocalStructureType& structure); // for debugging

private:
  void ExtractPyObject(
      bp::object inObject, LocalStructureType& outStructure);
};

#endif // LOCALSTRUCTURE_H
