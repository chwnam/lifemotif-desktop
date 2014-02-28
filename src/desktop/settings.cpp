#include "settings.h"

#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QFileInfo>

#include "exceptions.h"
#include "utils.h"

namespace LifeMotif {

QSettings* Settings::settings = 0;

Settings::Settings()
{
}

Settings::~Settings()
{
  if (settings) {
    delete settings;
    settings = NULL;
  }
}

void Settings::Init(QApplication* app)
{
  QCoreApplication::setOrganizationName(Config::organizationName);
  QCoreApplication::setOrganizationDomain(Config::organizationDomain);
  QCoreApplication::setApplicationName(Config::applicationName);
  QCoreApplication::setApplicationVersion(QString::fromUtf8(Config::Version()));

  if (settings == NULL) {
    settings = new QSettings(
          QSettings::IniFormat,
          QSettings::UserScope,
          QString(Config::organizationName),
          QString(Config::applicationName),
          app);

    qDebug() << "Setting file path:" << settings->fileName();
  }

  // init value check
  CheckSettings();

  qDebug("all settings are checked and inited");
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

}
