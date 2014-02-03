#ifndef BASEPYTHONWRAPPER_H
#define BASEPYTHONWRAPPER_H

#include <boost/python.hpp>
#include <string>
#include <cassert>

namespace bp = boost::python;

class BasePythonWrapper
{
public:
  BasePythonWrapper(
          const std::string& _moduleName,
          const std::string& _className);

protected:
  std::string moduleName;
  std::string className;

  bp::object module;
  bp::object instance;

public:
  void
  InitWrapper(PyObject* pArgs);

private:
  inline bp::object
  CreateObject(PyObject* pyObj)
  {
    return bp::object(bp::handle<>(pyObj));
  }
};

#endif // BASEPYTHONWRAPPER_H
