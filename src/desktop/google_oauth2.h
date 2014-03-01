#ifndef LIFEMOTIF_GOOGLE_OAUTH2_H
#define LIFEMOTIF_GOOGLE_OAUTH2_H

#include <QObject>
#include <QString>
#include <QUrl>
#include <QUrlQuery>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

#include "imap_manager.h"

namespace LifeMotif {

class GoogleOAuth2 : public QObject
{
  Q_OBJECT

public:
  explicit GoogleOAuth2(QObject *parent = 0);

  QUrl GetAuthorizationUrl(
    const QByteArray& authUri,
    const QByteArray& clientId,
    const QByteArray& redirectUri);

  void Authorize(
    const QByteArray& code,
    const QUrl&       tokenUri,
    const QByteArray& clientId,
    const QByteArray& clientSecret,
    const QByteArray& redirectUri);

  void Refresh(
    const QUrl&       tokenUri,
    const QByteArray& clientId,
    const QByteArray& clientSecret,
    const QByteArray& refreshToken);

  void Revoke(const QByteArray& accessToken);

signals:
  void GoogleAuthorized (const QVariantMap&);
  void GoogleTokenRefreshed (const QVariantMap&);

private slots:
  // slot for authentication
  void AuthReplyFinished();

  // slot for token refresh
  void RefreshReplyFinished();

  // slot for revoke
  void RevokeReplyFinished();

  void ReplyError(QNetworkReply::NetworkError error);

  

private:
  void ParseAuthReplyJson(const QByteArray& json);
  void ParseRefresReplyJson(const QByteArray& json);
  void ReplyCleanUp(const char* slotToDisconnect);  

private:
  QNetworkAccessManager *manager;
  QNetworkReply         *reply;
};

}

#endif // LIFEMOTIF_GOOGLE_OAUTH2_H
