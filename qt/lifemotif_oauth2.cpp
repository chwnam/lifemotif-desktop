#include "lifemotif_oauth2.h"
#include "lifemotif_client_info.h"
#include <QUrlQuery>


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
