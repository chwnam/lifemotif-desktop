#ifndef LIFEMOTIF_GOOGLE_OAUTH2_CREDENTIAL_H
#define LIFEMOTIF_GOOGLE_OAUTH2_CREDENTIAL_H

#include <QByteArray>
#include <QVariantMap>

class LifeMotifGoogleOAuth2Credential
{
public:
  LifeMotifGoogleOAuth2Credential() {}

  inline QByteArray& TokenExpiry()  { return _tokenExpiry;  }
  inline QByteArray& AccessToken()  { return _accessToken;  }
  inline QByteArray& RefreshToken() { return _refreshToken; }
  inline QByteArray& TokenType()    { return _tokenType;    }
  inline int&        ExpiresIn()    { return _expiresIn;    }

  inline const QByteArray& TokenExpiry()  const { return TokenExpiry();  }
  inline const QByteArray& AccessToken()  const { return AccessToken();  }
  inline const QByteArray& RefreshToken() const { return RefreshToken(); }
  inline const QByteArray& TokenType()    const { return TokenType();    }
  inline const QByteArray& ExpiresIn()    const { return ExpiresIn();    }

  bool IsExpired() const { return  false; }

  QVariantMap ToMap();

  static LifeMotifGoogleOAuth2Credential
    FromMap(const QVariantMap& map);

  static LifeMotifGoogleOAuth2Credential
    FromGoogleReplyJson(const QByteArray& json);

private:
  QByteArray _tokenExpiry;
  QByteArray _accessToken;
  QByteArray _refreshToken;
  QByteArray _tokenType;
  int        _expiresIn;
};

#endif //  LIFEMOTIF_GOOGLE_OAUTH2_CREDENTIAL_H
