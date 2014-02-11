#ifndef LOCALSTRUCTURE_H
#define LOCALSTRUCTURE_H

#include <boost/python.hpp>
#include <string>
#include <vector>
#include <map>

#include "message_types.h"

namespace bp = boost::python;

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
