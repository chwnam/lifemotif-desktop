#ifndef GOOGLEOAUTH2CONTROLLER_H
#define GOOGLEOAUTH2CONTROLLER_H

#include "oauth2wrapper.h"

class GoogleOauth2Wrapper : public Oauth2Wrapper
{
public:
    GoogleOauth2Wrapper(
            const std::string& _moduleName,
            const std::string& _className);

    std::string
    GetAuthorizationURL(const std::string& secret_path);

    bp::object
    MakeCredentials(const std::string& code);

    void
    SetCredentials(const std::string& storageName, bp::object& credentials);

    bp::object
    GetCredentials(const std::string& storageName);

    bp::object
      ImapAuthenticate(
            const std::string& storageName,
            const std::string& emailAddress,
            const int debugLevel = 0);

    void
      Revoke(const std::string& storageName);

};

#endif // GOOGLEOAUTH2CONTROLLER_H
