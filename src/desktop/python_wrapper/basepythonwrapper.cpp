#include "basepythonwrapper.h"

BasePythonWrapper::BasePythonWrapper(
        const std::string& _moduleName,
        const std::string& _className)
    : moduleName(_moduleName), className(_className)
{
}

void
BasePythonWrapper::InitWrapper(PyObject* pArgs)
{
  bp::object classObj;

  module = CreateObject(PyImport_ImportModule(moduleName.c_str()));
  assert(module.ptr());

  classObj = CreateObject(
                PyObject_GetAttrString(module.ptr(), className.c_str()));
  assert(classObj);

  instance = CreateObject(
                    PyObject_CallObject(classObj.ptr(), pArgs));
}
