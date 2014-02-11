#include "mainwindow.h"
#include <QApplication>
#include "lifemotif_config.h"
#include "lifemotif_settings.h"

#include <QTextStream>
#include <QFile>

void test_read_email()
{

}

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
  LifeMotifSettings::Init(LIFEMOTIF_DEFAULT_SETTINGS_PATH);

  if (Py_IsInitialized() == false) {
    Py_Initialize();
    PythonWorkAround();
  }
}

void PythonWorkAround()
{
  const QString& qscriptpath = LifeMotifSettings::PythonScriptPath();
  std::string runCode;  

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
