#ifndef LIFEMOTIF_SETTINGS_H
#define LIFEMOTIF_SETTINGS_H

#include <QApplication>
#include <QSettings>
#include <QString>
#include <QVariant>

#include "utils.h"


namespace LifeMotif {
class Settings
{
public:
  ~Settings();

private:
  Settings();

  // ini: qt-based configuration
  static QSettings* settings;

private:
  static void CheckSettings();

  static void SetDefaultValue(
      const QString& key, const QVariant& defaultValue = QVariant());

  static void ValidateSubdiretory(
    const QString& dir, const QString& subdir);

public:
  static void Init(QApplication* app);

  /***************************************************************************/
  /* INI configuration                                                       */
  /***************************************************************************/

  /***************************************************************************/
  // use_file_cache: use file cache.
  inline static bool UseFileCache() {
    return settings->value(QString("use_file_cache")).toBool();
  }
  inline static void UseFileCache(bool value) {
    settings->setValue(QString("use_file_cache"), value);
  }

  /***************************************************************************/
  // cache_dir: path to cache directory, storing email messages.
  inline static const QString CacheDir() {
    return settings->value(QString("cache_dir")).toString();
  }
  inline static void CacheDir(const QString& value) {
    settings->setValue(QString("cache_dir"), value);
  }

  /***************************************************************************/
  // local_structure_dir:
  inline static const QString LocalStructureDir() {
    return settings->value(QString("local_structure_dir")).toString();
  }
  inline static void LocalStructureDir(const QString& value) {
    settings->setValue(QString("local_structure_dir"), value);
  }

  /***************************************************************************/
  // google/email_address:
  inline static const QString GoogleEmailAddress() {
    return settings->value(QString("google/email_address")).toString();
  }
  inline static void GoogleEmailAddress(const QString& value) {
    settings->setValue(QString("google/email_address"), value);
  }

  /***************************************************************************/
  // google/mailbox:
  inline static const QString GoogleMailbox() {
    return settings->value(QString("google/mailbox")).toString();
  }
  inline static void GoogleMailbox(const QString& value) {
    settings->setValue(QString("google/mailbox"), value);
  }

  /***************************************************************************/
  // google/client_id: client ID from Google Developers Console
  inline static const QString GoogleClientId() {
    return settings->value(QString("google/client_id")).toString();
  }
  inline static void GoogleClientId(const QString& value) {
    settings->setValue(QString("cache_dir"), value);
  }

  /***************************************************************************/
  // google/client_secret: client secret from Google Developers Console
  inline static const QString GoogleClientSecret() {
    return settings->value(QString("google/client_secret")).toString();
  }
  inline static void GoogleClientSecret(const QString& value) {
    settings->setValue(QString("google/client_secret"), value);
  }

  /***************************************************************************/
  // google/redirect_uri: redirect uri from Google Developers Console
  inline static const QString GoogleRedirectUri() {
    return settings->value(QString("google/redirect_uri")).toString();
  }
  inline static void GoogleRedirectUri(const QString& value) {
    settings->setValue(QString("google/redirect_uri"), value);
  }

  /***************************************************************************/
  // google/access_token: Google OAuth2 access token
  inline static const QString GoogleAccessToken() {
    return settings->value(QString("google/access_token")).toString();
  }
  inline static void GoogleAccessToken(const QString& value) {
    settings->setValue(QString("google/access_token"), value);
  }

  /***************************************************************************/
  // google/refresh_token: Google OAuth2 refresh token
  inline static const QString GoogleRefreshToken() {
    return settings->value(QString("google/refresh_token")).toString();
  }
  inline static void GoogleRefreshToken(const QString& value) {
    settings->setValue(QString("google/refresh_token"), value);
  }

  /***************************************************************************/
  // google/token_expiry: Google OAuth2 token expiry
  inline static const QString GoogleTokenExpiry() {
    return settings->value(QString("google/token_expiry")).toString();
  }
  inline static void GoogleTokenExpiry(const QString& value) {
    settings->setValue(QString("google/token_expiry"), value);
  }
};
}

#endif // LIFEMOTIF_SETTINGS_H
