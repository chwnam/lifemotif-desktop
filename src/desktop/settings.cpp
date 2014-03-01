#include "settings.h"

#include <QCoreApplication>
#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QVariantMap>

#include "exceptions.h"
#include "utils.h"

namespace LifeMotif {

Settings::Settings(QObject* parent)
  : QObject(parent)
{
  settings = new QSettings(
    QSettings::IniFormat,
    QSettings::UserScope,
    QString(Config::organizationName),
    QString(Config::applicationName),
    parent);

  qDebug() << "INI file path:" << settings->fileName();
}

void Settings::Init()
{  
  // init value check
  CheckSettings();
  qDebug() << "All settings are checked and initialized.";
}

bool Settings::IsTokenAvailable() const
{
  return GoogleAccessToken().isEmpty()  == false &&
         GoogleRefreshToken().isEmpty() == false;
}

bool Settings::IsGoogleTokenAvailable() const
{
  return GoogleAccessToken().isEmpty()  == false &&
         GoogleRefreshToken().isEmpty() == false;
}

bool Settings::IsGoogleTokenExpired() const
{
  const QDateTime now = QDateTime::currentDateTimeUtc();
  const QDateTime expiry
    = QDateTime::fromString(GoogleTokenExpiry(), Qt::ISODate);

  qDebug() << "now:" << now;
  qDebug() << "expire:" << expiry;
  qDebug() << "expiry < now:" << (expiry < now);

  return expiry < now;
}

void Settings::CheckSettings()
{
  // check all setting values are defined
  // if not defined, assign its default value
  const QString dirstr = QFileInfo(settings->fileName()).absolutePath();

  QDir dir(dirstr);
  if (dir.exists() == false) {
    if (dir.mkpath(dirstr) == false) {
      throw InvalidSetting("Cannot create path " + dirstr.toStdString());
    }
  }

  // use_file_cache
  SetDefaultValue(
    QString("use_file_cache"),
    QVariant(Config::DefaultValues::useFileCache));

  // cache_dir
  const QString fileCacheDirDefault
      = Utils::JoinPath(
        dirstr,
        QString(Config::DefaultValues::fileCacheDir));

  SetDefaultValue(
    QString("cache_dir"),
    QVariant(fileCacheDirDefault));

  // local_structure_dir
  const QString localSturctureDirDefault
      = Utils::JoinPath(
        dirstr,
        QString(Config::DefaultValues::localStructureDir));

  SetDefaultValue(
    QString("local_structure_dir"),
    QVariant(localSturctureDirDefault));

  SetDefaultValue(
    QString("google/scope"),
    QVariant(Config::DefaultValues::Google::scope));

  SetDefaultValue(
    QString("google/auth_uri"),
    QVariant(Config::DefaultValues::Google::authUri));

  SetDefaultValue(
    QString("google/token_uri"),
    QVariant(Config::DefaultValues::Google::tokenUri));

  SetDefaultValue(
    QString("google/revoke_uri"),
    QVariant(Config::DefaultValues::Google::revokeUri));

  SetDefaultValue(QString("google/email_address"));
  SetDefaultValue(QString("google/mailbox"));

//  SetDefaultValue(
//    QString("google/client_id"),
//    QVariant(Config::DefaultValues::Google::clientId));

//  SetDefaultValue(
//    QString("google/client_secret"),
//    QVariant(Config::DefaultValues::Google::clientSecret));

  SetDefaultValue(
    QString("google/redirect_uri"),
    QVariant(Config::DefaultValues::Google::redirectUri));

  SetDefaultValue(QString("google/access_token"));
  SetDefaultValue(QString("google/refresh_token"));
  SetDefaultValue(QString("google/token_expiry"));

  // check all values are valid
  ValidateSubdiretory(dir, CacheDir());
  ValidateSubdiretory(dir, LocalStructureDir());
}

void
Settings::SetDefaultValue(
  const QString& key,
  const QVariant& defaultValue)
{
  if (settings->contains(key) == false) {
    settings->setValue(key, defaultValue);
  }
}

void
Settings::ValidateSubdiretory(
  QDir& dir, const QString& subdir)
{
  // if subdir does not exist, it is ok.
  // try to create it
  if (dir.exists(subdir) == false) {
    if (dir.mkdir(subdir) == false) {
      std::string msg;
      msg += "Cannot create cache directory '";
      msg += dir.absolutePath().toStdString();
      msg += "' at ";
      msg += dir.absolutePath().toStdString();
      throw InvalidSetting(msg);
    }
    qDebug() << "'" << subdir << "' has been created at" << dir.absolutePath();
  }  

  dir.cd(subdir);
  if (Utils::IsDirectoryAccessibleWritable(dir.absolutePath()) == false) {
    std::string msg;
    msg += "'" + dir.absolutePath().toStdString() + "' ";
    msg += " is not a directory or inaccessible.";
    throw InvalidSetting(msg);
  }
}

/***************************************************************************/
// use_file_cache: use file cache.
const bool Settings::UseFileCache() const {
  return settings->value(QString("use_file_cache")).toBool();
}
void Settings::UseFileCache(bool value) {
  settings->setValue(QString("use_file_cache"), value);
  emit UseFileCacheChanged(value);
}

/***************************************************************************/
// cache_dir: path to cache directory, storing email messages.
const QString Settings::CacheDir() const {
  return settings->value(QString("cache_dir")).toString();
}
void Settings::CacheDir(const QString& value) {
  settings->setValue(QString("cache_dir"), value);
  emit CacheDirChanged(value);
}

/***************************************************************************/
// local_structure_dir:
const QString Settings::LocalStructureDir() const {
  return settings->value(QString("local_structure_dir")).toString();
}
void Settings::LocalStructureDir(const QString& value) {
  settings->setValue(QString("local_structure_dir"), value);
  emit LocalStructureDirChanged(value);
}

/***************************************************************************/
// google/scope (gmail):
const QByteArray Settings::GoogleScope() const {
  return settings->value(QString("google/scope")).toByteArray();
}

/***************************************************************************/
// google/auth_uri:
const QByteArray Settings::GoogleAuthUri() const {
  return settings->value(QString("google/auth_uri")).toByteArray();
}
void Settings::GoogleAuthUri(const QByteArray& value) {
  settings->setValue(QString("google/auth_uri"), value);
  emit GoogleAuthUriChanged(value);
}

/***************************************************************************/
// google/token_uri:
const QByteArray Settings::GoogleTokenUri() const {
  return settings->value(QString("google/token_uri")).toByteArray();
}
void Settings::GoogleTokenUri(const QByteArray& value) {
  settings->setValue(QString("google/token_uri"), value);
  emit GoogleTokenUriChanged(value);
}

/***************************************************************************/
// google/revoke_uri:
const QByteArray Settings::GoogleRevokeUri() const {
  return settings->value(QString("google/revoke_uri")).toByteArray();
}
void Settings::GoogleRevokeUri(const QByteArray& value) {
  settings->setValue(QString("google/revoke_uri"), value);
  emit GoogleRevokeUriChanged(value);
}

/***************************************************************************/
// google/email_address:
const QString Settings::GoogleEmailAddress() const {
  return settings->value(QString("google/email_address")).toString();
}
void Settings::GoogleEmailAddress(const QString& value) {
  settings->setValue(QString("google/email_address"), value);  
  emit GoogleEmailAddressChanged(value);
}

/***************************************************************************/
// google/mailbox:
const QString Settings::GoogleMailbox() const {
  return settings->value(QString("google/mailbox")).toString();
}
void Settings::GoogleMailbox(const QString& value) {
  settings->setValue(QString("google/mailbox"), value);  
  emit GoogleMailboxChanged(value);  
}

/***************************************************************************/
// google/client_id: client ID from Google Developers Console
const QByteArray Settings::GoogleClientId() const {
  return Config::DefaultValues::Google::clientId;
}
//  void Settings::GoogleClientId(const QByteArray& value) {
//    settings->setValue(QByteArray("cache_dir"), value);
//  }

/***************************************************************************/
// google/client_secret: client secret from Google Developers Console
const QByteArray Settings::GoogleClientSecret() const {
  return Config::DefaultValues::Google::clientSecret;
}
//  void Settings::GoogleClientSecret(const QByteArray& value) {
//    settings->setValue(QByteArray("google/client_secret"), value);
//  }

/***************************************************************************/
// google/redirect_uri: redirect uri from Google Developers Console
const QByteArray Settings::GoogleRedirectUri() const {
  return settings->value(QString("google/redirect_uri")).toByteArray();
}
void Settings::GoogleRedirectUri(const QByteArray& value) {
  settings->setValue(QString("google/redirect_uri"), value);
  emit GoogleRedirectUriChanged(value);
}

/***************************************************************************/
// google/access_token: Google OAuth2 access token
const QByteArray Settings::GoogleAccessToken() const {
  return settings->value(QString("google/access_token")).toByteArray();
}
void Settings::GoogleAccessToken(const QByteArray& value) {
  settings->setValue(QString("google/access_token"), value);
  emit GoogleAccessTokenChanged(value);
}

/***************************************************************************/
// google/refresh_token: Google OAuth2 refresh token
const QByteArray Settings::GoogleRefreshToken() const {
  return settings->value(QString("google/refresh_token")).toByteArray();
}
void Settings::GoogleRefreshToken(const QByteArray& value) {
  settings->setValue(QString("google/refresh_token"), value);  
  emit GoogleRefreshTokenChanged(value);
}

/***************************************************************************/
// google/token_expiry: Google OAuth2 token expiry
const QString Settings::GoogleTokenExpiry() const {
  return settings->value(QString("google/token_expiry")).toString();
}
void Settings::GoogleTokenExpiry(const QString& value) {
  settings->setValue(QString("google/token_expiry"), value);
  emit GoogleTokenExpiryChanged(value);
}

void Settings::GoogleAuthorized(const QVariantMap& replyJsonMap)
{
  // calculate expiry
  const QString expiry
    = GetTokenExpiry(replyJsonMap[QString("expires_in")].toInt());

  GoogleAccessToken  (replyJsonMap["access_token"].toByteArray());
  GoogleRefreshToken (replyJsonMap["refresh_token"].toByteArray());
  GoogleTokenExpiry  (expiry);
}

void Settings::GoogleTokenRefreshed (const QVariantMap& replyJsonMap)
{
  // update our refresh token and expiry
  const QString expiry
    = GetTokenExpiry(replyJsonMap[QString("expires_in")].toInt());

  GoogleAccessToken  (replyJsonMap["access_token"].toByteArray());
  GoogleTokenExpiry  (expiry);
}

QString Settings::GetTokenExpiry(const int expiresIn) const
{
  return
    QDateTime::currentDateTimeUtc().addSecs(expiresIn).toString(Qt::ISODate);
}

}
