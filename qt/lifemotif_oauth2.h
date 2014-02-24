#ifndef LIFEMOTIF_OAUTH2_H
#define LIFEMOTIF_OAUTH2_H

#include <QString>
#include <QUrl>

#include "lifemotif_google_oauth2_credential.h"

class LifeMotifOauth2
{
public:
  LifeMotifOauth2();

  QUrl
    GetAuthorizationUrl(const QString& secretPath);

  LifeMotifGoogleOAuth2Credential
    MakeCredentials(
      const QString& secretPath, const QString& code);

  void
    SetCredentials(const QString& storageName);

  void
    GetCredentials(const QString& storageName);

  void
    ImapAuthenticate(
      const QString& storageName,
      const QString& emailAddress,
      const int debugLevel = 0);

private:
  static const QString endpoint;
};

#endif // LIFEMOTIF_OAUTH2_H
