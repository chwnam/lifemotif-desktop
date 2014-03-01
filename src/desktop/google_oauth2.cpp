#include "google_oauth2.h"

#include <QFile>
#include <QJsonDocument>

#include "settings.h"
#include "utils.h"


namespace LifeMotif {

GoogleOAuth2::GoogleOAuth2(QObject *parent)
  : QObject(parent)
{
  manager = new QNetworkAccessManager(this);
  reply = NULL;
}

QUrl
  GoogleOAuth2::GetAuthorizationUrl(
  const QByteArray& authUri,
  const QByteArray& clientId,
  const QByteArray& redirectUri)
{
  QUrl       url;
  QUrlQuery  query;

  const QByteArray responseType ("code");
  const QByteArray scope        ("https://mail.google.com/");

  query.addQueryItem("client_id",     QUrl::toPercentEncoding(clientId));
  query.addQueryItem("redirect_uri",  QUrl::toPercentEncoding(redirectUri));
  query.addQueryItem("response_type", QUrl::toPercentEncoding(responseType));
  query.addQueryItem("scope",         QUrl::toPercentEncoding(scope));

  url.setUrl(authUri);
  url.setQuery(query);

  return url;
}

void GoogleOAuth2::Authorize(
  const QByteArray& code,
  const QUrl&       tokenUri,
  const QByteArray& clientId,
  const QByteArray& clientSecret,
  const QByteArray& redirectUri)
{
  QNetworkRequest request;

  const QByteArray grantType ("authorization_code");

  //http://qt-project.org/doc/qt-5.0/qtnetwork/qnetworkrequest.html
  QByteArray content;

  content.append ("code="          + code         + '&');
  content.append ("client_id="     + clientId     + '&');
  content.append ("client_secret=" + clientSecret + '&');
  content.append ("redirect_uri="  + redirectUri  + '&');
  content.append ("grant_type="    + grantType);

  request.setUrl(tokenUri);
  request.setHeader(
      QNetworkRequest::ContentTypeHeader,
      "application/x-www-form-urlencoded");
  request.setHeader(
      QNetworkRequest::ContentLengthHeader,
      content.size());

  //http://qt-project.org/doc/qt-5.0/qtnetwork/qnetworkaccessmanager.html
  //http://qt-project.org/doc/qt-5.0/qtnetwork/qnetworkreply.html
  
  if (manager) {
    reply = manager->post(request, content);

    if (reply) {
      connect(reply, SIGNAL(finished()), this, SLOT(ReplyFinished()));
      connect(
        reply, SIGNAL(error(QNetworkReply::NetworkError)),
        this, SLOT(ReplyError(QNetworkReply::NetworkError)));
      //WaitForSignal(manager, SIGNAL(finished(QNetworkReply*)), 5000);
      WaitForSignal(reply, SIGNAL(finished()), 5000);
    }
  }
}

bool
  GoogleOAuth2::WaitForSignal(
    QObject *sender, const char *signal, int timeout)
{
  QEventLoop loop;
  QTimer timer;

  timer.setInterval(timeout);
  timer.setSingleShot(true);

  loop.connect(sender, signal, SLOT(quit()));
  loop.connect(&timer, SIGNAL(timeout()), SLOT(quit()));

  timer.start();
  loop.exec();

  return timer.isActive();
}

void GoogleOAuth2::ReplyError(QNetworkReply::NetworkError error)
{
  qDebug() << "Error code:" << error;
}

void GoogleOAuth2::ReplyFinished()
{
    // header debugging code
//  const QList<QNetworkReply::RawHeaderPair >& pairs = reply->rawHeaderPairs();
//  qDebug() << "Pairs size:" << pairs.size();
//  for(QList<QNetworkReply::RawHeaderPair >::const_iterator it = pairs.cbegin(); it != pairs.cend(); ++it) {
//    qDebug() << it->first << it->second;
//  }

  ParseReplyJson(reply->readAll());
  ReplyCleanUp(SLOT(ReplyFinished()));
}

void GoogleOAuth2::Revoke(const QByteArray& accessToken)
{
  // https://accounts.google.com/o/oauth2/revoke?token=(token)
  // token can be an access token or a refresh token.
  // success: 200 repsonse, failure: 400 response

  QUrl      url(QString("https://accounts.google.com/o/oauth2/revoke"));
  QUrlQuery query;

  query.addQueryItem(QString("token"), accessToken);
  url.setQuery(query);

  qDebug() << "Revoke URL: " << url;

  if (reply = manager->get(QNetworkRequest(url))) {
    connect(
      reply, SIGNAL(finished()),
      this, SLOT(RevokeReplyFinished()));
    connect(
      reply, SIGNAL(error(QNetworkReply::NetworkError)),
      this, SLOT(ReplyError(QNetworkReply::NetworkError)));
    WaitForSignal(reply, SIGNAL(finished()), 5000);
  }
  ReplyCleanUp(SLOT(RevokeReplyFinished()));
}

void GoogleOAuth2::RevokeReplyFinished()
{
  const int statusCode
    = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

  switch (statusCode)
  {
  case 200:
    qDebug() << "Server returned 200. Revoke on server success.";
    break;

  case 400:
    qDebug() << "Server returned 400. Revoke on server fail.";
  }
}

void GoogleOAuth2::ReplyCleanUp(const char* slotToDisconnect)
{
  if (reply) {
    reply->close();
    disconnect(reply, SIGNAL(finished()), this, slotToDisconnect);
    disconnect(
      reply, SIGNAL(error(QNetworkReply::NetworkError)),
      this, SLOT(ReplyError(QNetworkReply::NetworkError)));
    reply->deleteLater();
    reply = NULL;
  }
}

QByteArray
  GoogleOAuth2::GetImapAuthString(
  const QString&    emailAddress,
  const QByteArray& accessToken)
{
  QByteArray authString;
  const char a = 0x1;

  authString += "user=" + emailAddress.toUtf8() + a;
  authString += "auth=Bearer ";
  authString += accessToken + a + a;

  return authString.toBase64();
}

void
  GoogleOAuth2::RefreshToken()
{
}

void
  GoogleOAuth2::ImapAuthenticate(
  const QString&    emailAddress,
  const QByteArray& accessToken)
{  
  if (IsTokenExpired()) {
    RefreshToken();
  }

  // LifeMotifImap* imap = new LifeMotifImap();
  const QByteArray authString = GetImapAuthString(emailAddress, accessToken);

  // host: imap.google.com
  // port: 993
  // SSL: always true
  // imap->Connect("imap.google", 993, true);
  // imap->Send(authString);

  qDebug() << "authString:" << authString;
}

void
GoogleOAuth2::ParseReplyJson(const QByteArray& json)
{
  const QJsonDocument replyJson = QJsonDocument::fromJson(json);
  if (replyJson.isObject()) {
    emit GoogleAuthorized(replyJson.toVariant().toMap());
  }
}

}
