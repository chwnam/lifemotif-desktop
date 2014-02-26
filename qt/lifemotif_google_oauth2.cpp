#include "lifemotif_google_oauth2.h"
#include "lifemotif_google_client_info.h"

#include <QDateTime>
#include <QJsonDocument>

#include "lifemotif_utils.h"

LifeMotifGoogleOauth2::LifeMotifGoogleOauth2(QObject *parent)
  : QObject(parent)
{
  manager = new QNetworkAccessManager(this);
  reply = NULL;
}

QUrl
  LifeMotifGoogleOauth2::GetAuthorizationUrl(const QString& secretPath)
{
  LifeMotifGoogleClientInfo clientInfo(secretPath);
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

void
  LifeMotifGoogleOauth2::MakeCredentials(
    const QString& secretPath, const QString& code)
{
  LifeMotifGoogleClientInfo clientInfo(secretPath);
  QNetworkRequest request;

  const QString &tokenUri     = clientInfo.TokenUri();
  const QString &clientId     = clientInfo.ClientId();
  const QString &clientSecret = clientInfo.ClientSecret();
  const QString &redirectUri  = clientInfo.RedirectUri();
  const QString &grantType    = QString("authorization_code");

  //http://qt-project.org/doc/qt-5.0/qtnetwork/qnetworkrequest.html
  QByteArray content;

  content += "code="          + code.toUtf8()         + '&';
  content += "client_id="     + clientId.toUtf8()     + '&';
  content += "client_secret=" + clientSecret.toUtf8() + '&';
  content += "redirect_uri="  + redirectUri.toUtf8()  + '&';
  content += "grant_type="    + grantType.toUtf8();

  request.setUrl(QUrl(tokenUri));
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

bool LifeMotifGoogleOauth2::WaitForSignal(QObject *sender, const char *signal, int timeout)
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

void LifeMotifGoogleOauth2::ReplyError(QNetworkReply::NetworkError error)
{
  /* when you've got an error:
    Error!
    Error code: 299 (QNetworkReply::UnknownContentError)
    == Reply ==
    Pairs size: 11
    "Cache-Control" "no-cache, no-store, max-age=0, must-revalidate"
    "Pragma" "no-cache"
    "Expires" "Fri, 01 Jan 1990 00:00:00 GMT"
    "Date" "Tue, 25 Feb 2014 14:35:52 GMT"
    "Content-Type" "application/json"
    "X-Content-Type-Options" "nosniff"
    "X-Frame-Options" "SAMEORIGIN"
    "X-XSS-Protection" "1; mode=block"
    "Server" "GSE"
    "Alternate-Protocol" "443:quic"
    "Transfer-Encoding" "chunked"
    "{
      "error" : "invalid_grant",
      "error_description" : "Invalid code."
    }"
  */
  qDebug() << "Error code:" << error;
}

void LifeMotifGoogleOauth2::ReplyFinished()
{
    // header debugging code
//  const QList<QNetworkReply::RawHeaderPair >& pairs = reply->rawHeaderPairs();
//  qDebug() << "Pairs size:" << pairs.size();
//  for(QList<QNetworkReply::RawHeaderPair >::const_iterator it = pairs.cbegin(); it != pairs.cend(); ++it) {
//    qDebug() << it->first << it->second;
//  }

  credential
      = LifeMotifGoogleOAuth2Credential::FromGoogleReplyJson(reply->readAll());

  // cleanup
  reply->close();
  disconnect(reply, SIGNAL(finished()), this, SLOT(ReplyFinished()));
  disconnect(
    reply, SIGNAL(error(QNetworkReply::NetworkError)),
    this, SLOT(ReplyError(QNetworkReply::NetworkError)));
  reply->deleteLater();
  reply = NULL;
}

void
  LifeMotifGoogleOauth2::SetCredentials(const QString& storageName)
{
  LifeMotifUtils::SaveJson(storageName, credential.ToMap());
}

void
  LifeMotifGoogleOauth2::GetCredentials(const QString& storageName)
{
  bool isQVariantMap;
  QVariant map;
  
  map = LifeMotifUtils::LoadJson(storageName, &isQVariantMap);

  if (isQVariantMap) {
    credential
      = LifeMotifGoogleOAuth2Credential::FromMap(map.toMap());
  }
}
