#ifndef LOCALSTRUCTUREWRAPPER_H
#define LOCALSTRUCTUREWRAPPER_H

#include "basepythonwrapper.h"

class LocalStructure;

class LocalStructureWrapper : public BasePythonWrapper
{
public:
  LocalStructureWrapper(
      const std::string& _moduleName,
      const std::string& _className);

  void
  Build(const std::string& fileName,
        bp::object& structure);

  bp::object
  Load(const std::string& fileName);
};


#endif // LOCALSTRUCTUREWRAPPER_H
