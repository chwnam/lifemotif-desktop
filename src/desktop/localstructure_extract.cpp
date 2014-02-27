#include "localstructure_extract.h"
#include <iostream>

LocalStructureExtract::LocalStructureExtract(
    bp::object& inObject, LocalStructureType& outStructure)
{
  ExtractPyObject(inObject, outStructure);
}

void
LocalStructureExtract::ExtractPyObject(
    bp::object inObject, LocalStructureType& outStructure)
{
  if (PyDict_Check(inObject.ptr()) == false) {
    return;
  }

  if (outStructure.empty() == false) {
    outStructure.clear();
  }

  bp::dict obj = bp::extract<bp::dict>(inObject);

  const bp::object iterItems = obj.iteritems();
  unsigned long
      iterCount = bp::extract<unsigned long>(obj.attr("__len__")());

  for(unsigned long i = 0; i < iterCount; ++i)
  {
    bp::object item = iterItems.attr("next")();
    bp::object dictKey, dictValue;

    dictKey = item[0];
    dictValue = item[1];

    if (PyTuple_Check(dictValue.ptr())) {

      unsigned long
          tuplelen = bp::extract<unsigned long>(dictValue.attr("__len__")());

      if (tuplelen == 2) {

        bp::object thrElem = dictValue[0];
        bp::object msgElem = dictValue[1];

        // container key
        DateType
            dateKey = DateType(bp::extract<const char*>(dictKey));

        // dateKey should be found only once.
        // however, if we find the key more than twice, then overwrite the value.
        if (outStructure.find(dateKey) == outStructure.end()) {
          outStructure[dateKey] = MessageGroup();
        }

        // thread id
        outStructure[dateKey].threadId = bp::extract<ThrIdType>(thrElem);

        // message id
        unsigned long
            listlen = bp::extract<unsigned long>(msgElem.attr("__len__")());
        for(unsigned long j = 0; j < listlen; ++j) {
          outStructure[dateKey].messageIds.push_back(
                bp::extract<MsgIdType>(msgElem[j]));
        } // for..
      } // if (tuplelen)
    } // if (PyTuple_Check
  } // for ..
}

void LocalStructureExtract::PrintStructure(const LocalStructureType& structure)
{
  for(LocalStructureType::const_iterator cit = structure.begin();
      cit != structure.end();
      ++cit) {
    const DateType date = cit->first;
    const MessageGroup& group = cit->second;

    std::cout << "time: " << date << '\n';
    std::cout << "\tthread id: " << group.threadId << '\n';

    for(std::size_t i = 0; i < group.messageIds.size(); ++i) {
      std::cout << "\tmessage id: ";
      std::cout << group.messageIds[i] << '\n';
    }
  }
}
