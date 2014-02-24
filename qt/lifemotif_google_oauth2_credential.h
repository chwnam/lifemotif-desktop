#ifndef LIFEMOTIF_GOOGLE_OAUTH2_CREDENTIAL_H
#define LIFEMOTIF_GOOGLE_OAUTH2_CREDENTIAL_H

#include <QString>

class LifeMotifGoogleOAuth2Credential
{
public:
  LifeMotifGoogleOAuth2Credential() {}

  inline QString& TokenExpiry()  { return _tokenExpiry;  }
  inline QString& AccessToken()  { return _accessToken;  }
  inline QString& RefreshToken() { return _refreshToken; }
  inline int&     ExpiresIn()    { return _expiresIn;    }

  inline const QString& TokenExpiry()  const { return TokenExpiry();  }
  inline const QString& AccessToken()  const { return AccessToken();  }
  inline const QString& RefreshToken() const { return RefreshToken(); }
  inline const QString& ExpiresIn()    const { return ExpiresIn();    }

  bool IsExpired() const { return  false; }

private:
  QString _tokenExpiry;
  QString _accessToken;
  QString _refreshToken;
  int     _expiresIn;
};

#endif //  LIFEMOTIF_GOOGLE_OAUTH2_CREDENTIAL_H