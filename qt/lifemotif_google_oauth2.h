#ifndef LIFEMOTIF_OAUTH2_H
#define LIFEMOTIF_OAUTH2_H

#include <QObject>
#include <QString>
#include <QUrl>
#include <QUrlQuery>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>
#include <QTimer>

#include "lifemotif_google_oauth2_credential.h"


class LifeMotifGoogleOauth2 : public QObject
{
  Q_OBJECT

public:
  LifeMotifGoogleOauth2(QObject *parent = 0);

  QUrl
    GetAuthorizationUrl(const QString& secretPath);

  void
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

  void Revoke(const QString& storageName);

private slots:
  // slot for authentication
  void ReplyFinished();
  void ReplyError(QNetworkReply::NetworkError error);

  // slot for revoke
  void RevokeReplyFinished();

private:
  bool WaitForSignal(QObject *sender, const char *signal, int timeout);
  void ParseReplyJson();

  void ReplyCleanUp(const char* slotToDisconnect);

private:
  QNetworkAccessManager *manager;
  QNetworkReply         *reply;

  LifeMotifGoogleOAuth2Credential credential;
};

#endif // LIFEMOTIF_OAUTH2_H
