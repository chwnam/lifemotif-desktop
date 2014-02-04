#include "oauth2wrapper.h"

Oauth2Wrapper::Oauth2Wrapper(
    const std::string& _moduleName,
    const std::string& _className)
  : BasePythonWrapper(_moduleName, _className)
{
}

std::string
Oauth2Wrapper::GrantUserPermission(const std::string &url)
{
  bp::object codeObject;

  std::cout << "Testing GrantUserPermission...\n";

  codeObject = CreateObject(
                PyObject_CallMethod(
                  instance.ptr(),
                  "grant_user_permission",
                  "s",
                  url.c_str()));

  return std::string(
            bp::extract<const char*>(
              Utf8EncodedString(codeObject)));
}
