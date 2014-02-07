#include "localstructure.h"
#include <iostream>

LocalStructure::LocalStructure(bp::object& structureObject)
{
  ExtractPyObject(structureObject);
}

void
LocalStructure::ExtractPyObject(bp::object structureObject)
{
  if (PyDict_Check(structureObject.ptr()) == false) {
    return;
  }

  structure.clear();

  bp::dict obj = bp::extract<bp::dict>(structureObject);

  const bp::object iterItems = obj.iteritems();
  unsigned long
      iterCount = bp::extract<unsigned long>(obj.attr("__len__")());

  //std::cout << "There are " << iterCount << " items!\n";
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

        // container
        DateType
            dateKey = DateType(bp::extract<const char*>(dictKey));
        structure[dateKey] = DailyContainer();

        // thread id
        structure[dateKey].threadId = bp::extract<ThrIdType>(thrElem);
        //std::cout << structure[dateKey].threadId << std::endl;

        // message id
        unsigned long
            listlen = bp::extract<unsigned long>(msgElem.attr("__len__")());
        for(unsigned long j = 0; j < listlen; ++j) {
          structure[dateKey].messageIds.push_back(
                bp::extract<MsgIdType>(msgElem[j]));
        }
      }
    }
  }
}

void LocalStructure::PrintStructure()
{
  for(StructureType::iterator it = structure.begin();
      it != structure.end();
      ++it) {
    DateType date = it->first;
    DailyContainer& cont = it->second;

    std::cout << "time: " << date << '\n';
    std::cout << "\tthread id: " << cont.threadId << '\n';

    for(std::size_t i = 0; i < cont.messageIds.size(); ++i) {
      std::cout << "\tmessage id: ";
      std::cout << cont.messageIds[i] << '\n';
    }
  }
}
