#include "lifemotif_oauth2.h"
#include "lifemotif_client_info.h"
#include <QUrlQuery>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QScopedPointer>


LifeMotifOauth2::LifeMotifOauth2()
{
}

QUrl
  LifeMotifOauth2::GetAuthorizationUrl(const QString& secretPath)
{
  LifeMotifClientInfo clientInfo(secretPath);
  QUrl                url;
  QUrlQuery           query;

  const QString &authUri     = clientInfo.AuthUri();
  const QString &clientId    = clientInfo.ClientId();
  const QString &redirectUri = clientInfo.RedirectUri();
  const QString responseType = QString("code");
  const QString scope        = QString("https://mail.google.com/");

  query.addQueryItem("client_id", QUrl::toPercentEncoding(clientId));
  query.addQueryItem("redirect_uri", QUrl::toPercentEncoding(redirectUri));
  query.addQueryItem("response_type", QUrl::toPercentEncoding(responseType));
  query.addQueryItem("scope", QUrl::toPercentEncoding(scope));

  url.setUrl(authUri);
  url.setQuery(query);

  return url;
}

LifeMotifGoogleOAuth2Credential
  LifeMotifOauth2::MakeCredentials(
    const QString& secretPath, const QString& code)
{
  LifeMotifClientInfo clientInfo(secretPath);
  QNetworkRequest request;

  const QString &tokenUri     = clientInfo.TokenUri();
  const QString &clientId     = clientInfo.ClientId();
  const QString &clientSecret = clientInfo.ClientSecret();
  const QString &redirectUri  = clientInfo.RedirectUri();
  const QString &grantType    = QString("authorization_code");

  //http://qt-project.org/doc/qt-5.0/qtnetwork/qnetworkrequest.html
  request.setRawHeader(
    "code", QUrl::toPercentEncoding(code));
  request.setRawHeader(
    "client_id", QUrl::toPercentEncoding(clientId));
  request.setRawHeader(
    "client_secret", QUrl::toPercentEncoding(clientSecret));
  request.setRawHeader(
    "redirect_uri", QUrl::toPercentEncoding(redirectUri));
  request.setRawHeader(
    "grant_type", QUrl::toPercentEncoding(grantType));

  request.setUrl(QUrl(tokenUri));

  //http://qt-project.org/doc/qt-5.0/qtnetwork/qnetworkaccessmanager.html
  //http://qt-project.org/doc/qt-5.0/qtnetwork/qnetworkreply.html
  
  typedef QScopedPointer<QNetworkAccessManager> AccessManagerPtr;
  typedef QScopedPointer<QNetworkReply>         ReplyPtr;

  AccessManagerPtr access(new QNetworkAccessManager);
  ReplyPtr         reply(access->post(request, QByteArray()));
  
  qDebug() << "access_token:" << reply->rawHeader("access_token");
  qDebug() << "refresh_token:" << reply->rawHeader("refresh_token");
  qDebug() << "expires_in:" << reply->rawHeader("expires_in");
  qDebug() << "token_type:" << reply->rawHeader("token_type");

  return LifeMotifGoogleOAuth2Credential();
}
