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
      connect(
        reply, SIGNAL(finished()),
        this, SLOT(AuthReplyFinished()));

      connect(
        reply, SIGNAL(error(QNetworkReply::NetworkError)),
        this, SLOT(ReplyError(QNetworkReply::NetworkError)));

      //WaitForSignal(manager, SIGNAL(finished(QNetworkReply*)), 5000);
      Utils::WaitForSignal(reply, SIGNAL(finished()), 5000);
    }
  }
}

void GoogleOAuth2::ReplyError(QNetworkReply::NetworkError error)
{
  qDebug() << "Error code:" << error;
}

void GoogleOAuth2::AuthReplyFinished()
{
    // header debugging code
//  const QList<QNetworkReply::RawHeaderPair >& pairs = reply->rawHeaderPairs();
//  qDebug() << "Pairs size:" << pairs.size();
//  for(QList<QNetworkReply::RawHeaderPair >::const_iterator it = pairs.cbegin(); it != pairs.cend(); ++it) {
//    qDebug() << it->first << it->second;
//  }

  ParseAuthReplyJson(reply->readAll());
  ReplyCleanUp(SLOT(AuthReplyFinished()));
}

void GoogleOAuth2::RefreshReplyFinished()
{
  ParseRefresReplyJson(reply->readAll());
  ReplyCleanUp(SLOT(RefreshReplyFinished()));
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
    Utils::WaitForSignal(reply, SIGNAL(finished()), 5000);
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

void
  GoogleOAuth2::Refresh(
  const QUrl&       tokenUri,
  const QByteArray& clientId,
  const QByteArray& clientSecret,
  const QByteArray& refreshToken)
{
  QNetworkRequest request;  
  QByteArray content;

  content.append ("client_id="     + clientId     + '&');
  content.append ("client_secret=" + clientSecret + '&');
  content.append ("refresh_token=" + refreshToken + '&');
  content.append ("grant_type="    + QByteArray("refresh_token"));

  request.setUrl(tokenUri);

  request.setHeader(
    QNetworkRequest::ContentTypeHeader,
    "application/x-www-form-urlencoded");

  request.setHeader(
    QNetworkRequest::ContentLengthHeader,
    content.size());

  if (manager) {
    reply = manager->post(request, content);

    if (reply) {
      connect(
        reply, SIGNAL(finished()),
        this, SLOT(RefreshReplyFinished()));

      connect(
        reply, SIGNAL(error(QNetworkReply::NetworkError)),
        this, SLOT(ReplyError(QNetworkReply::NetworkError)));      

      Utils::WaitForSignal(reply, SIGNAL(finished()), 30000);
    }
  }
}

void
GoogleOAuth2::ParseAuthReplyJson(const QByteArray& json)
{
  const QJsonDocument replyJson = QJsonDocument::fromJson(json);
  if (replyJson.isObject()) {
    emit GoogleAuthorized(replyJson.toVariant().toMap());
  }
}

void
  GoogleOAuth2::ParseRefresReplyJson(const QByteArray& json)
{
  const QJsonDocument replyJson = QJsonDocument::fromJson(json);
  if (replyJson.isObject()) {
    emit GoogleTokenRefreshed(replyJson.toVariant().toMap());
  }
}

}
