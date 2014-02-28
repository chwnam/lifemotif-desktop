#include "settings.h"

#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QFileInfo>

#include "config.h"
#include "lifemotif_exceptions.h"
#include "lifemotif_utils.h"

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
  }

  // init value check
  CheckSettings();

  qDebug("all settings are checked and inited");
}

void Settings::CheckSettings()
{
  // check all setting values are defined
  // if not defined, assign its default value
  const QString dir = QFileInfo(settings->fileName()).canonicalPath();

  // use_file_cache
  SetDefaultValue(
    QString("use_file_cache"),
    QVariant(Config::DefaultValues::useFileCache));

  // cache_dir
  const QString fileCacheDirDefault
      = LifeMotifUtils::JoinPath(
        dir,
        QString(Config::DefaultValues::fileCacheDir));

  SetDefaultValue(
    QString("cache_dir"),
    QVariant(fileCacheDirDefault));

  // local_structure_dir
  const QString localSturctureDirDefault
      = LifeMotifUtils::JoinPath(
        dir,
        QString(Config::DefaultValues::localStructureDir));

  SetDefaultValue(
    QString("local_structure_dir"),
    QVariant(localSturctureDirDefault));

  SetDefaultValue(QString("google/email_address"));
  SetDefaultValue(QString("google/mailbox"));

  SetDefaultValue(
    QString("google/client_id"),
    QVariant(Config::DefaultValues::Google::clientId));

  SetDefaultValue(
    QString("google/client_secret"),
    QVariant(Config::DefaultValues::Google::clientSecret));

  SetDefaultValue(
    QString("google/redirect_uri"),
    QVariant(Config::DefaultValues::Google::redirectUri));

  SetDefaultValue(QString("google/access_token"));
  SetDefaultValue(QString("google/refresh_token"));
  SetDefaultValue(QString("google/token_expiry"));

  // check all values are valid
  ValidateSubdiretory(settings->fileName(), CacheDir());
  ValidateSubdiretory(settings->fileName(), LocalStructureDir());
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
  const QString& dir, const QString& subdir)
{
  QDir d(dir);

  if (d.exists() == false) {
    std::string msg;
    msg += "path '" + d.absolutePath().toStdString() + "' ";
    msg += "does not exist.";
    throw LifeMotifException(msg);
  }

  // if subdir does not exist, it is ok.
  // try to create it
  if (d.exists(subdir) == false) {
    if (d.mkdir(subdir) == false) {
      std::string msg;
      msg += "Cannot create cache directory '";
      msg += d.absolutePath().toStdString();
      msg += "' at ";
      msg += d.absolutePath().toStdString();
      throw LifeMotifException(msg);
    }
  }

  d.cd(subdir);
  if (LifeMotifUtils::IsDirectoryAccessibleWritable(d.absolutePath())) {
    std::string msg;
    msg += "'" + d.absolutePath().toStdString() + "' ";
    msg += " is not a directory or inaccessible.";
    throw LifeMotifException(msg);
  }
}
}
