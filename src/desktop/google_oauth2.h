#ifndef LIFEMOTIF_GOOGLE_OAUTH2_H
#define LIFEMOTIF_GOOGLE_OAUTH2_H

#include <QObject>
#include <QString>
#include <QUrl>
#include <QUrlQuery>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>
#include <QTimer>

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

  void ImapAuthenticate(
    const QString&    emailAddress,
    const QByteArray& accessToken);
  
  void Revoke(const QByteArray& accessToken);

signals:
  void GoogleAuthorized (const QVariantMap& replyJsonMap);

private slots:
  // slot for authentication
  void ReplyFinished();
  void ReplyError(QNetworkReply::NetworkError error);

  // slot for revoke
  void RevokeReplyFinished();

private:
  void ParseReplyJson(const QByteArray& json);
  void RefreshToken();
  void ReplyCleanUp(const char* slotToDisconnect);  
  bool WaitForSignal(QObject *sender, const char *signal, int timeout);

  QByteArray GetImapAuthString(
    const QString&    emailAddress,
    const QByteArray& accessToken);

  bool IsTokenExpired() const {return false; }

private:
  QNetworkAccessManager *manager;
  QNetworkReply         *reply;
};

}

#endif // LIFEMOTIF_GOOGLE_OAUTH2_H
