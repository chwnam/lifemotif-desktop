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
  GoogleOAuth2(QObject *parent = 0);

  QUrl GetAuthorizationUrl();
  void Authorize(const QByteArray& code);
  void ImapAuthenticate();
  void Revoke();

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

  QByteArray GetImapAuthString();
  bool IsTokenExpired() const {return false; }

private:
  QNetworkAccessManager *manager;
  QNetworkReply         *reply;
};

}

#endif // LIFEMOTIF_GOOGLE_OAUTH2_H
