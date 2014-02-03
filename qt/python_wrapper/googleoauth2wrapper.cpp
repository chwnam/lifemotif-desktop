#include "googleoauth2wrapper.h"
#include <cassert>

GoogleOauth2Wrapper::GoogleOauth2Wrapper(
        const std::string& _moduleName,
        const std::string& _className)
    : BasePythonWrapper(_moduleName, _className)
{
  assert(Py_IsInitialized());
  InitWrapper(NULL);
}

GoogleOauth2Wrapper::~GoogleOauth2Wrapper()
{

}

std::string
GoogleOauth2Wrapper::GetAuthorizationURL(const std::string& secret_path)
{

}

std::string
GoogleOauth2Wrapper::MakeCredentials(const std::string& code)
{

}

void
GoogleOauth2Wrapper::SetCredentials(
    const std::string& storageName,
    bp::object& credentials)
{

}

bp::object
GoogleOauth2Wrapper::GetCredentials(const std::string& storageName)
{

}

bp::object
GoogleOauth2Wrapper::ImapAuthenticate(
    const std::string& storageName,
    const std::string& emailAddress,
    const int debugLevel)
{

}
