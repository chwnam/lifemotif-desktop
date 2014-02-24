#ifndef LIFEMOTIF_OAUTH2_H
#define LIFEMOTIF_OAUTH2_H

#include <QString>

class LifeMotifOauth2
{
public:
  LifeMotifOauth2();

  QString
    GetAuthorizationUrl(const QString& secretPath);

  void
    MakeCredentials(const QString& code);

  void
    SetCredentials(const QString& storageName);

  void
    GetCredentials(const QString& storageName);

  void
    ImapAuthenticate(
      const QString& storageName,
      const QString& emailAddress,
      const int debugLevel = 0);
};

#endif // LIFEMOTIF_OAUTH2_H
