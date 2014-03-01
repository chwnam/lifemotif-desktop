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

private:
  void CheckSettings();

  void SetDefaultValue(
      const QString& key, const QVariant& defaultValue = QVariant());

  void ValidateSubdiretory(QDir& dir, const QString& subdir);

public:
  void Init();
  bool IsTokenAvailable();

  /* general settings */
  bool UseFileCache();
  void UseFileCache(bool value);

  const QString CacheDir();
  void          CacheDir(const QString& value);

  const QString LocalStructureDir();
  void          LocalStructureDir(const QString& value);


  /* google settings */
  const QByteArray GoogleAuthUri();
  void             GoogleAuthUri(const QByteArray& value);

  const QByteArray GoogleTokenUri();
  void             GoogleTokenUri(const QByteArray& value);

  const QByteArray GoogleRevokeUri();
  void             GoogleRevokeUri(const QByteArray& value);

  const QString GoogleEmailAddress();
  void          GoogleEmailAddress(const QString& value);

  const QString GoogleMailbox();
  void          GoogleMailbox(const QString& value);

  const QByteArray GoogleClientId();
  //void             GoogleClientId(const QByteArray& value);

  const QByteArray GoogleClientSecret();
  // void             GoogleClientSecret(const QByteArray& value);

  const QByteArray GoogleRedirectUri();
  void             GoogleRedirectUri(const QByteArray& value);

  const QByteArray GoogleAccessToken();
  void             GoogleAccessToken(const QByteArray& value);

  const QByteArray GoogleRefreshToken();
  void             GoogleRefreshToken(const QByteArray& value);

  const QString GoogleTokenExpiry();
  void          GoogleTokenExpiry(const QString& value);
};

}

#endif // LIFEMOTIF_SETTINGS_H
