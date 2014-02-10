#include "pythonscriptsettings.h"
#include <QFile>
#include <QJsonDocument>
#include <exception>
#include <iostream>

QVariantMap* PythonScriptSettings::instance = NULL;

class JSONFileNotFoundException : public std::exception {
public:
  JSONFileNotFoundException(QString _fileName)
    : std::exception()
  {
    description = "File '";
    description += _fileName;
    description += "' not found.";
  }
  ~JSONFileNotFoundException() throw() {}

  const char* what() const throw() {
    return description.toStdString().c_str();
  }
private:
  QString description;
};

class JSONFileInvalidException : public std::exception {
public:
  JSONFileInvalidException(QString _fileName)
    : std::exception()
  {
    description = "JSON file '";
    description += _fileName;
    description += "' is invalid.";
  }
  ~JSONFileInvalidException() throw() {}
  const char* what() const throw() {
    return description.toStdString().c_str();
  }
private:
  QString description;
};

PythonScriptSettings::PythonScriptSettings()
{
}

PythonScriptSettings::~PythonScriptSettings()
{
  if (instance == NULL) {
    delete instance;
    instance = NULL;
  }
}

void PythonScriptSettings::Init(const QString& path)
{
  std::cout << "Initializing " << path.toStdString() << std::endl;

  if (instance == NULL) {
    QFile file;
    file.setFileName(path);
    if (file.exists() == false) {
      throw JSONFileNotFoundException(path);
    }

    file.open(QIODevice::ReadOnly);
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    if (doc.isNull()) {
      throw JSONFileInvalidException(path);
    }

    instance = new QVariantMap(doc.toVariant().toMap());
    if (instance  == NULL) throw std::bad_alloc();
  }
}

QVariantMap& PythonScriptSettings::GetSettings()
{
  return *instance;
}
