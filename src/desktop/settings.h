#ifndef LIFEMOTIF_SETTINGS_H
#define LIFEMOTIF_SETTINGS_H

#include <QApplication>
#include <QByteArray>
#include <QObject>
#include <QSettings>
#include <QString>
#include <QVariant>

#include "config.h"
#include "utils.h"

namespace LifeMotif {

class Settings : public QObject
{
  Q_OBJECT

public:
  explicit Settings(QObject* parent = 0);

private:
  // ini: qt-based configuration
  QSettings* settings;

signals:
  /* general */
  void UseFileCacheChanged(bool newValue);
  void CacheDirChanged(const QString& newValue);
  void LocalStructureDirChanged(const QString& newValue);

  /* google settings */
  void GoogleAuthUriChanged       (const QByteArray& newValue);
  void GoogleTokenUriChanged      (const QByteArray& newValue);
  void GoogleRevokeUriChanged     (const QByteArray& newValue);
  void GoogleEmailAddressChanged  (const QString&    newValue);
  void GoogleMailboxChanged       (const QString&    newValue);
  void GoogleRedirectUriChanged   (const QByteArray& newValue);
  void GoogleAccessTokenChanged   (const QByteArray& newValue);
  void GoogleRefreshTokenChanged  (const QByteArray& newValue);
  void GoogleTokenExpiryChanged   (const QString&    newValue);

private slots:
  void GoogleAuthorized (const QVariantMap& replyJsonMap);
  void GoogleTokenRefreshed (const QVariantMap& replyJsonMap);

private:
  void CheckSettings();

  void SetDefaultValue(
      const QString& key, const QVariant& defaultValue = QVariant());

  void ValidateSubdiretory(QDir& dir, const QString& subdir);

  QString GetTokenExpiry(const int expiresIn) const;

public:
  void Init();
  bool IsTokenAvailable() const;

  bool IsGoogleTokenAvailable() const;
  bool IsGoogleTokenExpired() const;

  /* general settings */
  const bool UseFileCache() const;
  void UseFileCache(bool value);

  const QString CacheDir() const;
  void          CacheDir(const QString& value);

  const QString LocalStructureDir() const;
  void          LocalStructureDir(const QString& value);


  /* google settings */
  const QByteArray GoogleScope() const;

  const QByteArray GoogleAuthUri() const;
  void             GoogleAuthUri(const QByteArray& value);

  const QByteArray GoogleTokenUri() const;
  void             GoogleTokenUri(const QByteArray& value);

  const QByteArray GoogleRevokeUri() const;
  void             GoogleRevokeUri(const QByteArray& value);

  const QString GoogleEmailAddress() const;
  void          GoogleEmailAddress(const QString& value);

  const QString GoogleMailbox() const;
  void          GoogleMailbox(const QString& value);

  const QByteArray GoogleClientId() const;
  //void             GoogleClientId(const QByteArray& value);

  const QByteArray GoogleClientSecret() const;
  // void             GoogleClientSecret(const QByteArray& value);

  const QByteArray GoogleRedirectUri() const;
  void             GoogleRedirectUri(const QByteArray& value);

  const QByteArray GoogleAccessToken() const;
  void             GoogleAccessToken(const QByteArray& value);

  const QByteArray GoogleRefreshToken() const;
  void             GoogleRefreshToken(const QByteArray& value);

  const QString GoogleTokenExpiry() const;
  void          GoogleTokenExpiry(const QString& value);
};

}

#endif // LIFEMOTIF_SETTINGS_H
