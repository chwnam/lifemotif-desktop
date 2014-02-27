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
GoogleOauth2Wrapper::GetAuthorizationURL(const std::string& secretPath)
{
  bp::object urlObject;

  urlObject = CreateObject(
                PyObject_CallMethod(
                  instance.ptr(),
                  "get_authorization_url",
                  "s", secretPath.c_str()));

  return std::string(
            bp::extract<const char*>(
              Utf8EncodedString(urlObject)));
}

bp::object
GoogleOauth2Wrapper::MakeCredentials(const std::string& code)
{
  bp::object credentialsObj;

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
  bp::object imapObject;

  imapObject = CreateObject(
                  PyObject_CallMethod(
                    instance.ptr(),
                    "imap_authenticate",
                    "ssi",
                    storageName.c_str(),
                    emailAddress.c_str(),
                    debugLevel));
  
  // imap_authenticate may fail
  // if 'storage_name' which holds tokens does not exist.
  return imapObject.ptr() == Py_None ? bp::object() : imapObject;
}

void
GoogleOauth2Wrapper::Revoke(const std::string& storageName)
{
  bp::object revokeFunc = instance.attr("revoke");

  revokeFunc(storageName);
}
