#ifndef LIFEMOTIF_SETTINGS_H
#define LIFEMOTIF_SETTINGS_H

#include <QSharedPointer>
#include <QSettings>
#include <QVariant>

#include "lifemotif_utils.h"

typedef QSharedPointer<QSettings> QSettingsPtr;
typedef QSharedPointer<QVariantMap> QVariantMapPtr;

class LifeMotifSettings
{
private:
  LifeMotifSettings();
  ~LifeMotifSettings();

  // ini: qt-based configuration
  static QSettingsPtr iniInstance;

  // json: python script config.
  static QVariantMapPtr jsonInstance;

  static QVariantMap LoadJson(const QString& path);

private:
  static void IniValueCheck(const QString& iniPath);
  static void JsonValueCheck();
  static void SetIniDefault();

public:
  /***************************************************************************/
  /* INI configuration                                                       */
  /***************************************************************************/
  // python_script_path: where all python scripts are
  inline static const QString
  PythonScriptPath() {
    return iniInstance->value("python_script_path").toString();
  }
  inline static void PythonScriptPath(const QString& value) {
    iniInstance->setValue("python_script_path", value);
  }

  /***************************************************************************/
  // python_config: path to python script configuration file (json)
  inline static const QString
  PythonConfig() {
    return iniInstance->value("python_config").toString();
  }
  inline static void
  PythonConfig(const QString& value) {
    iniInstance->setValue("python_config", value);
  }

  /***************************************************************************/
  // use_file_cache: use file cache.
  inline static bool UseFileCache() {
    return iniInstance->value("use_file_cache").toBool();
  }
  inline static void UseFileCache(bool value) {
    iniInstance->setValue("use_file_cache", value);
  }

  /***************************************************************************/
  // cache_dir: path to cache directory, storing email messages.
  inline static const QString CacheDir() {
    return iniInstance->value("cache_dir").toString();
  }
  inline static void CacheDir(const QString& value) {
    iniInstance->setValue("cache_dir", value);
  }

  /***************************************************************************/
  /* JSON configuration                                                      */
  /***************************************************************************/
  // secret_path
  inline static const QString
  SecretPath(bool combineWithPythonScriptPath = true) {
    return (combineWithPythonScriptPath ?
            LifeMotifUtils::JoinPath(
                PythonScriptPath(),
                (*jsonInstance)["secret_path"].toString()) :
            (*jsonInstance)["secret_path"].toString());
  }
  inline static void SecretPath(const QString& value) {
    (*jsonInstance)["secret_path"] = value;
  }

  /***************************************************************************/
  // storage_name
  inline static const QString
  StorageName(bool combineWithPythonScriptPath = true) {
    return (combineWithPythonScriptPath ?
              LifeMotifUtils::JoinPath(
                  PythonScriptPath(),
                  (*jsonInstance)["storage_name"].toString()) :
            (*jsonInstance)["storage_name"].toString());
  }
  inline static void StorageName(const QString& value) {
    (*jsonInstance)["storage_name"] = value;
  }

  /***************************************************************************/
  // label
  inline static const QString
  Label() {
    return (*jsonInstance)["label"].toString();
  }
  inline static void
  Label(const QString& value) {
    (*jsonInstance)["label"] = value;
  }

  /***************************************************************************/
  // email_address
  inline static const QString
  EmailAddress() {
    return (*jsonInstance)["email_address"].toString();
  }
  inline static void
  EmailAddress(const QString& value) {
    (*jsonInstance)["email_address"] = value;
  }

  /***************************************************************************/
  // local_database
  inline static const QString
  LocalStructure(bool combineWithPythonScriptPath = true) {
    return (combineWithPythonScriptPath ?
              LifeMotifUtils::JoinPath(
                  PythonScriptPath(),
                  (*jsonInstance)["local_structure"].toString()) :
            (*jsonInstance)["local_structure"].toString());
  }
  inline static void
  LocalStructure(const QString& value) {
    (*jsonInstance)["local_structure"] = value;
  }

  /***************************************************************************/
  // debug_level
  inline static int DebugLevel() {
    return (*jsonInstance)["debug_level"].toInt();
  }
  inline static void DebugLevel(const int value) {
    (*jsonInstance)["local_structure"] = value;
  }

  static void Init(const QString& path);
};

#endif // LIFEMOTIF_SETTINGS_H
