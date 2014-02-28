#include "google_client_info.h"

#include <QJsonDocument>
#include <QArrayData>
#include <QVariantList>
#include <QDebug>

#include "exceptions.h"

namespace LifeMotif {
GoogleClientInfo::GoogleClientInfo()
{
  LoadClientInfo(clientInfoPath);
}

void
GoogleClientInfo::LoadClientInfo()
{
  QFile file;

  file.setFileName(clientInfoPath);
  file.open(QIODevice::ReadOnly|QIODevice::Text);

  if (file.isReadable()) {

    QJsonDocument info = QJsonDocument::fromJson(file.readAll());

    if (info.isEmpty() || info.isNull()) {
      throw LifeMotifInvalidSetting(std::string("JSON file parsing error!"));
    }

    if (info.isObject()) {
      // read values from map
      SetInfo(info.toVariant().toMap());
    }
    file.close();
  }
}

void
GoogleClientInfo::SetInfo(const QVariantMap& map)
{
  //for(QVariantMap::const_iterator it = map.cbegin(); it != map.end(); ++it) {
  //  qDebug() << "key:" << *it; //-> << "\tvalue:" << it->second;
  //}

  // the json file should have "installed" key: variant value
  const QVariantMap& installed = map["installed"].toMap();

  _authUri      = installed["auth_uri"].toString();
  _tokenUri     = installed["token_uri"].toString();
  _clientId     = installed["client_id"].toString();
  _clientSecret = installed["client_secret"].toString();

  // redirect_uris key has a list
  const QVariantList& redirectUris = installed["redirect_uris"].toList();

  _redirectUri = redirectUris[0].toString(); // 0th element
}
}
