#include "lifemotif_google_oauth2_credential.h"

#include <QDateTime>
#include <QJsonDocument>
#include <QDebug>

QVariantMap
  LifeMotifGoogleOAuth2Credential::ToMap()
{
  QVariantMap map;

  map.insert(QString("access_token"),  AccessToken());
  map.insert(QString("token_type"),    TokenType());
  map.insert(QString("expires_in"),    ExpiresIn());
  map.insert(QString("refresh_token"), RefreshToken());
  map.insert(QString("expiry"),        TokenExpiry());

  return map;
}

LifeMotifGoogleOAuth2Credential
  LifeMotifGoogleOAuth2Credential::FromMap(const QVariantMap& map)
{
  LifeMotifGoogleOAuth2Credential credential;

  credential.AccessToken()  = map[QString("access_token")].toByteArray();
  credential.TokenType()    = map[QString("token_type")].toByteArray();
  credential.ExpiresIn()    = map[QString("expires_in")].toInt();
  credential.RefreshToken() = map[QString("refresh_token")].toByteArray();
  credential.TokenExpiry()  = map[QString("expiry")].toByteArray();

  return credential;
}

LifeMotifGoogleOAuth2Credential
  LifeMotifGoogleOAuth2Credential::FromGoogleReplyJson(const QByteArray& json)
{
  LifeMotifGoogleOAuth2Credential credential;

  const QJsonDocument replyJson = QJsonDocument::fromJson(json);
  
  if (replyJson.isObject()) {

    const QVariantMap replyMap = replyJson.toVariant().toMap();
    QVariantMap       credMap;

    // calculate expiry
    QDateTime expiry = QDateTime::currentDateTimeUtc();
    const int expiresIn = replyMap[QString("expires_in")].toInt();   

    expiry = expiry.addSecs(expiresIn);

    const QString at("access_token");
    const QString tt("token_type");
    const QString ei("expires_in");
    const QString rt("refresh_token");
    const QString ex("expiry");

    credMap[at] = replyMap[at].toByteArray();
    credMap[tt] = replyMap[tt].toByteArray();
    credMap[ei] = expiresIn;
    credMap[rt] = replyMap[rt].toByteArray();
    credMap[ex] = expiry.toString(Qt::ISODate).toUtf8();

    credential = FromMap(credMap);
  }

  return credential;
}
