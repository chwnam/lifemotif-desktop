#ifndef LOCALSTRUCTURE_H
#define LOCALSTRUCTURE_H

#include <boost/python.hpp>
#include <string>
#include <vector>
#include <map>

namespace bp = boost::python;

class LocalStructure
{
public:
  LocalStructure(bp::object& structureObject);

  typedef unsigned long long ThrIdType;
  typedef unsigned long long MsgIdType;
  typedef std::string DateType;

  // key: date
  // val: (thread_id, [message_ids])

  struct DailyContainer
  {
    ThrIdType threadId;
    std::vector<MsgIdType> messageIds;
  };

  typedef std::map<DateType, DailyContainer> StructureType;

  void PrintStructure();

private:
  void ExtractPyObject(bp::object structureObject);

  StructureType structure;
};

#endif // LOCALSTRUCTURE_H
