#ifndef GOOGLEOAUTH2CONTROLLER_H
#define GOOGLEOAUTH2CONTROLLER_H

#include "basepythonwrapper.h"

class GoogleOauth2Wrapper : public BasePythonWrapper
{
public:
    GoogleOauth2Wrapper(
            const std::string& _moduleName,
            const std::string& _className);
    ~GoogleOauth2Wrapper();

    std::string
    GetAuthorizationURL(const std::string& secret_path);

    std::string
    MakeCredentials(const std::string& code);

    void
    SetCredentials(const std::string& storageName, bp::object& credentials);

    bp::object
    GetCredentials(const std::string& storageName);

    bp::object ImapAuthenticate(
            const std::string& storageName,
            const std::string& emailAddress,
            const int debugLevel = 0);
};

#endif // GOOGLEOAUTH2CONTROLLER_H
