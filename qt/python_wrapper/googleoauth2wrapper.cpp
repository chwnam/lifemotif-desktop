#include "googleoauth2wrapper.h"
#include <cassert>

GoogleOauth2Wrapper::GoogleOauth2Wrapper(
        const std::string& _moduleName,
        const std::string& _className)
    : Oauth2Wrapper(_moduleName, _className)
{
  InitWrapper(NULL);
}

std::string
GoogleOauth2Wrapper::GetAuthorizationURL(const std::string& secret_path)
{
  bp::object urlObject;

  std::cout << "Testing AuthorizationURL...\n";

  urlObject = CreateObject(
                PyObject_CallMethod(
                  instance.ptr(),
                  "get_authorization_url",
                  "s", secret_path.c_str()));

  return std::string(
            bp::extract<const char*>(
              Utf8EncodedString(urlObject)));
}

bp::object
GoogleOauth2Wrapper::MakeCredentials(const std::string& code)
{
  bp::object credentialsObj;

  std::cout << "Testing MakeCredentials...\n";

  credentialsObj = CreateObject(
                PyObject_CallMethod(
                  instance.ptr(),
                  "make_credentials",
                  "s", code.c_str()));

  return credentialsObj;
}

void
GoogleOauth2Wrapper::SetCredentials(
    const std::string& storageName,
    bp::object& credentials)
{
  std::cout << "Testing SetCredentials...\n";

  assert(credentials.ptr());

  PyObject_CallMethod(
        instance.ptr(),
        "set_credentials",
        "sO",
        storageName.c_str(),
        credentials.ptr());
}

bp::object
GoogleOauth2Wrapper::GetCredentials(const std::string& storageName)
{
  std::cout << "Testing GetCredentials...\n";

  bp::object credentials;

  credentials = CreateObject(
                  PyObject_CallMethod(
                    instance.ptr(),
                    "get_credentials",
                    "s",
                    storageName.c_str()));

  return credentials;
}

bp::object
GoogleOauth2Wrapper::ImapAuthenticate(
    const std::string& storageName,
    const std::string& emailAddress,
    const int debugLevel)
{
  std::cout << "Testing ImapAuthenticate...\n";

  bp::object imapObject;

  imapObject = CreateObject(
                  PyObject_CallMethod(
                    instance.ptr(),
                    "imap_authenticate",
                    "ssi",
                    storageName.c_str(),
                    emailAddress.c_str(),
                    debugLevel));

  return imapObject;
}
