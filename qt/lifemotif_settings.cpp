#include "lifemotif_settings.h"

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QTextStream>

#include "lifemotif_config.h"
#include "lifemotif_exceptions.h"
#include "lifemotif_utils.h"

QSettingsPtr LifeMotifSettings::iniInstance = QSettingsPtr();
QVariantMapPtr LifeMotifSettings::jsonInstance = QVariantMapPtr();

LifeMotifSettings::LifeMotifSettings()
{
}

LifeMotifSettings::~LifeMotifSettings()
{
}

void LifeMotifSettings::Init(const QString& path)
{ 
  if (iniInstance == NULL) {
    iniInstance = QSettingsPtr(new QSettings(path, QSettings::IniFormat));
    if (iniInstance == NULL) throw std::bad_alloc();
  }

  // init value check
  IniValueCheck(path);

  if (jsonInstance == NULL) {
    QVariantMap map = LoadJson(PythonConfig());
    jsonInstance = QVariantMapPtr(new QVariantMap(map));
    if (jsonInstance == NULL) throw std::bad_alloc();
  }

  // json value check
  JsonValueCheck();

  qDebug("all settings are checked and inited");
}

QVariantMap LifeMotifSettings::LoadJson(const QString& path)
{
  QFile file(path);
  if (file.exists() == false) {
    throw FileNotFoundException(path.toStdString());
  }

  file.open(QIODevice::ReadOnly);
  QJsonDocument jsonDoc = QJsonDocument::fromJson(file.readAll());

  if (jsonDoc.isNull()) {
    throw FileInvalidException(path.toStdString());
  }
  qDebug() << ".json setting file" << path << "loaded.";
  return jsonDoc.toVariant().toMap();
}

void LifeMotifSettings::IniValueCheck(const QString& iniPath)
{
  if (LifeMotifUtils::Exists(iniPath) == false) {
    // ini path is missing. It is acceptable.
    // Use default value and go ahead
    qDebug() << iniPath << "is missing. Set to default values...";
    SetIniDefault();
  } else {
    qDebug() << ".ini setting file" << iniPath << "found.";
  }

  QString pythonScriptPath = PythonScriptPath();
  QString pythonConfig = PythonConfig();
  bool useFileCache = UseFileCache();
  QString cacheDir = CacheDir();

  if (LifeMotifUtils::IsDirectoryAccessible(pythonScriptPath) == false) {
    // Python script path is invalid. This is bad. Stop the program.
    std::string errMsg;
    errMsg = "Critical error - python_script_path: ";
    errMsg += "'" + pythonScriptPath.toStdString() + "'";
    errMsg += " is not a directory or not readable.";
    throw LifeMotifInvalidSetting(errMsg);
  }
  if (LifeMotifUtils::IsFileReadable(pythonConfig) == false) {
    // Python script configuration must be readable. Stop the program.
    std::string errMsg;
    errMsg = "Critical error - python_config: ";
    errMsg += "'" + pythonConfig.toStdString() += "'";
    errMsg += " is not a file or not readable.";
    throw LifeMotifInvalidSetting(errMsg);
  }
  if (useFileCache && LifeMotifUtils::IsDirectoryAccessible(cacheDir) == false) {
    // cache directory should be accesible.
    if (LifeMotifUtils::Exists(cacheDir) == false) {
      // this is acceptable. Create cacheDir and go ahead.
      QString absPath = QDir(cacheDir).absolutePath();
      QDir root = QDir::root();
      if(root.mkpath(absPath) == false) {
        // Failure to making cache directory is not acceptable.
        std::string errMsg;
        errMsg = "Critical error - cache_dir: ";
        errMsg += "cannot create cache directory: " + absPath.toStdString();
        throw LifeMotifInvalidSetting(errMsg);
      }
      qDebug() << cacheDir << "for file cache is missing. Newly created.";
    } else {
      // This is bad. Stop the program.
      std::string errMsg;
      errMsg = "Critical error - cache_dir: ";
      errMsg += "'" + cacheDir.toStdString() + "'";
      errMsg += " is not a directory or inaccessible.";
      throw LifeMotifInvalidSetting(errMsg);
    }
  }

  qDebug(".ini setting is ok!");
}

void LifeMotifSettings::SetIniDefault()
{
  const QString pythonScriptPath = QString(LIFEMOTIF_DEFAULT_PYTHON_SCRIPT_PATH);
  const QString pythonConfig = QString(LIFEMOTIF_DEFAULT_PYTHON_CONFIG);
  const bool useFileCache = LIFEMOTIF_DEFAULT_USE_FILE_CACHE;
  const QString cacheDir = QString(LIFEMOTIF_DEFAULT_CACHE_DIR);

  PythonScriptPath(pythonScriptPath);
  PythonConfig(pythonConfig);
  UseFileCache(useFileCache);
  CacheDir(cacheDir);
}

void LifeMotifSettings::JsonValueCheck()
{
  QString secretPath = SecretPath(true);
  QString storageName = StorageName(true);
  QString storPath = QFileInfo(storageName).dir().path();
  QString localStructure = LocalStructure(true);
  QString locPath = QFileInfo(localStructure).dir().path();

  if (LifeMotifUtils::IsFileReadable(secretPath) == false) {
    // secret_path is invalid. This is bad. Terminate.
    std::string errMsg;
    errMsg = "Critical error - secret_path: ";
    errMsg += "'" + secretPath.toStdString() + "'";
    errMsg += " is not a file or not readable.";
    throw LifeMotifInvalidSetting(errMsg);
  }

  if (LifeMotifUtils::IsDirectoryAccessibleWritable(storPath) == false) {
    // directory of storage name is invalid. This is bad. Terminate.
    std::string errMsg;
    errMsg = "Critical error - storage_name: ";
    errMsg += "'" + storPath.toStdString() + "'";
    errMsg += " is not a directory or not writable.";
    throw LifeMotifInvalidSetting(errMsg);
  } else {
    // directory is accessible and writable.
    // storage name should be readable/writable if it exists
    if (LifeMotifUtils::Exists(storageName) &&
        LifeMotifUtils::IsFileReadableWritable(storageName) == false ) {
      // storageName is not a file or not readable or not writable. Terminate.
      std::string errMsg;
      errMsg = "Critical error - storage_name: ";
      errMsg += "'" + storageName.toStdString() + "'";
      errMsg += " is not a file, or not readable/writable.";
      throw LifeMotifInvalidSetting(errMsg);
    }
  }

  if (LifeMotifUtils::IsDirectoryAccessibleWritable(locPath) == false) {
    // directory of local structure is invalid. This is bad. Terminate.
    std::string errMsg;
    errMsg = "Critical error - local_sturcture: ";
    errMsg += "'" + locPath.toStdString() + "'";
    errMsg += " is not a directory or not writable.";
    throw LifeMotifInvalidSetting(errMsg);
  } else {
    // directory is accessible and writable.
    // local structure shoud be readable and writable if it exists
    if (LifeMotifUtils::Exists(localStructure) &&
        LifeMotifUtils::IsFileReadableWritable(localStructure) == false) {
      // local structure is not a file or not readable or not writable. Terminate.
      std::string errMsg;
      errMsg = "Critical error - local_structure: ";
      errMsg += "'" + localStructure.toStdString() + "'";
      errMsg += " is not a file, or not readable/writable.";
      throw LifeMotifInvalidSetting(errMsg);
    }
  }

  qDebug(".json setting is ok!");
}
