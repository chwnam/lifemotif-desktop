#ifndef OAUTH2WRAPPER_H
#define OAUTH2WRAPPER_H

#include "basepythonwrapper.h"

class Oauth2Wrapper : public BasePythonWrapper
{
public:
  Oauth2Wrapper(
      const std::string& _moduleName,
      const std::string& _className);

  std::string
  GrantUserPermission(const std::string &url);
};

#endif // OAUTH2WRAPPER_H
