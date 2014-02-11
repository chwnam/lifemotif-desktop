#ifndef LIFEMOTIF_SETTINGS_H
#define LIFEMOTIF_SETTINGS_H

#include <boost/shared_ptr.hpp>
#include <QSettings>
#include <QVariant>

typedef boost::shared_ptr<QSettings> QSettingsPtr;
typedef boost::shared_ptr<QVariantMap> QVariantMapPtr;

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

public:

  /***************************************************************************/
  /* INI configuration                                                       */
  /***************************************************************************/
  // python_script_path: where all python scripts are
  inline static const QString PythonScriptPath() {
    return iniInstance->value("python_script_path").toString();
  }
  inline static void PythonScriptPath(const QString& value) {
    iniInstance->setValue("python_script_path", value);
  }

  /***************************************************************************/
  // python_config: path to python script configuration file (json)
  inline static const QString PythonConfig() {
    return iniInstance->value("python_config").toString();
  }
  inline static void PythonConfig(const QString& value) {
    iniInstance->setValue("python_config", value);
  }

  /***************************************************************************/
  /* JSON configuration                                                      */
  /***************************************************************************/
  // secret_path
  inline static const QString SecretPath() {
    return (*jsonInstance)["secret_path"].toString();
  }
  inline static void SecretPath(const QString& value) {
    (*jsonInstance)["secret_path"] = value;
  }

  /***************************************************************************/
  // storage_name
  inline static const QString StorageName() {
    return (*jsonInstance)["storage_name"].toString();
  }
  inline static void StorageName(const QString& value) {
    (*jsonInstance)["storage_name"] = value;
  }

  /***************************************************************************/
  // label
  inline static const QString Label() {
    return (*jsonInstance)["label"].toString();
  }
  inline static void Label(const QString& value) {
    (*jsonInstance)["label"] = value;
  }

  /***************************************************************************/
  // email_address
  inline static const QString EmailAddress() {
    return (*jsonInstance)["email_address"].toString();
  }
  inline static void EmailAddress(const QString& value) {
    (*jsonInstance)["email_address"] = value;
  }

  /***************************************************************************/
  // local_database
  inline static const QString LocalStructure() {
    return (*jsonInstance)["local_structure"].toString();
  }
  inline static void LocalStructure(const QString& value) {
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
