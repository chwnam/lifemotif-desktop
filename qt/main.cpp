#include "mainwindow.h"
#include <QApplication>
#include <QDir>
#include <iostream>
#include "./python_wrapper/googleoauth2wrapper.h"
#include "./python_wrapper/googleimapwrapper.h"
#include "./python_wrapper/localstructurewrapper.h"
#include "lifemotif_config.h"
#include "settingscontrol.h"
#include "pythonscriptsettings.h"
#include "localstructure.h"
#include "tester.h"

void Init();
void PythonWorkAround();
void Fin();

int main(int argc, char *argv[])
{
  int returnCode = EXIT_FAILURE;

  Init();

  try {

    QApplication a(argc, argv);
    MainWindow w;

    w.show();
    returnCode = a.exec();

  } catch(const bp::error_already_set&) {
    PyErr_Print();
  }

  Fin();
  return returnCode;
}

void Init()
{
  // this application's ini settings
  SettingsControl::Init(LIFEMOTIF_PREFERENCE_PATH);

  // retrieve python script's json settings
  QSettings& settings = SettingsControl::GetSettings();
  PythonScriptSettings::Init(settings.value("python_config").toString());

  if (Py_IsInitialized() == false) {
    Py_Initialize();
    PythonWorkAround();
  }

  // test
  //QVariantMap& jsonConfig = PythonScriptSettings::GetSettings();
  //std::cout << "label: " << jsonConfig["label"].toString().toStdString() << std::endl;
}

void PythonWorkAround()
{
  QSettings &settings = SettingsControl::GetSettings();
  QString qscriptpath;
  std::string runCode;

  qscriptpath = settings.value("python_script_path").toString();

  runCode += "import sys\n";
  runCode += "sys.path.append('" + qscriptpath.toStdString() + "')\n";
  runCode += "sys.path.append('.')\n";

  if(PyRun_SimpleString(runCode.c_str()) != 0) {
    std::cerr << "Could not run python workaround code. ";
    std::cerr << "Stop the program.";
    Fin();
    exit(EXIT_FAILURE);
  }
}

void Fin()
{
  if (Py_IsInitialized() == true) {
    Py_Finalize();
  }
}
