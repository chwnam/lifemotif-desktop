#ifndef BASEPYTHONWRAPPER_H
#define BASEPYTHONWRAPPER_H

#include <iostream> // DO NOT remove. Will make compile error!
#include <string>
#include <boost/python.hpp>

namespace bp = boost::python;

inline bp::object
CreateObject(PyObject* pyObj)
{
  return bp::object(bp::handle<>(pyObj));
}

inline bp::object
Utf8EncodedString(bp::object& obj)
{
  return bp::str(obj).encode("utf-8");
}

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

protected:
  void
  InitWrapper(PyObject* pArgs);
};

#endif // BASEPYTHONWRAPPER_H
