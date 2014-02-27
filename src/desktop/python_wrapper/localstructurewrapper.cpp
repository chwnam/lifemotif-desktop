#include "localstructurewrapper.h"

LocalStructureWrapper::LocalStructureWrapper(
    const std::string& _moduleName,
    const std::string& _className)
  : BasePythonWrapper(_moduleName, _className)
{
  InitWrapper(NULL);
}

void
LocalStructureWrapper::Build(const std::string& fileName,
      bp::object& structure)
{
  PyObject_CallMethod(
        instance.ptr(),
        "build",
        "sO",
        fileName.c_str(),
        structure.ptr());
}

bp::object
LocalStructureWrapper::Load(const std::string& fileName)
{
  bp::object structure;

  structure = CreateObject(
        PyObject_CallMethod(
          instance.ptr(),
          "load",
          "s",
          fileName.c_str()));

  return structure;
}
