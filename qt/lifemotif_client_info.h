#ifndef LIFEMOTIF_CLIENT_INFO_H
#define LIFEMOTIF_CLIENT_INFO_H

#include <QString>
#include <QVariantMap>


class LifeMotifClientInfo
{
public:
  LifeMotifClientInfo(const QString& clientInfoPath);

// read-only
//  inline       QString& AuthUri()       { return _authUri;      }
//  inline       QString& TokenUri()      { return _tokenUri;     }
//  inline       QString& ClientId()      { return _clientId;     }
//  inline       QString& ClientSecret()  { return _clientSecret; }
//  inline       QString& RedirectUri()   { return _redirectUri;  }

  inline const QString& AuthUri()      const { return _authUri;      }
  inline const QString& TokenUri()     const { return _tokenUri;     }
  inline const QString& ClientId()     const { return _clientId;     }
  inline const QString& ClientSecret() const { return _clientSecret; }
  inline const QString& RedirectUri()  const { return _redirectUri;  }

  // client info is immutable in this application.
  //void SaveClientInfo(const QString& clientInfoPath);

private:
  void LoadClientInfo(const QString& clientInfoPath);
  void SetInfo(const QVariantMap& map);

private:
  QString _authUri;
  QString _tokenUri;
  QString _clientId;
  QString _clientSecret;
  QString _redirectUri;
};

#endif // LIFEMOTIF_CLIENT_INFO_H
