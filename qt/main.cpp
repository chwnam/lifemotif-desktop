#include <boost/python.hpp>

#include "main_window.h"
#include <QApplication>
#include <QDebug>
#include <QMessageBox>
#include <cstdio>
#include <string>

#include "lifemotif_config.h"
#include "lifemotif_settings.h"
#include "lifemotif_exceptions.h"

void Init();
void PythonWorkAround();
void Fin();
void SetBuffer(FILE* stream, char* buffer, const int bufferSize);

int main(int argc, char *argv[])
{
  int returnCode = EXIT_FAILURE;
  QApplication a(argc, argv);

  try {
    Init();
    qDebug() << "Initialization complete. Ready to show main window...";

    MainWindow w;
    w.show();
    w.raise();
    w.activateWindow();

    returnCode = a.exec();
    qDebug() << "Main window is closed. Return code =" << returnCode;
  }

  // python script error
  catch(const bp::error_already_set&) {
    char*     errorBuffer = NULL;
    const int bufferSize = 4096;

    fflush(stderr);
    errorBuffer = (char*)malloc(sizeof(char)*bufferSize);
    SetBuffer(stderr, errorBuffer, bufferSize);
    PyErr_Print();

    QString message = QString("Program is unexpectedly terminated due to the python script error:\n");
    message += errorBuffer ? QString(errorBuffer) : QString("See console printout.");
    QMessageBox::critical(NULL, "Critical Error", message);

    fflush(stderr);
    free(errorBuffer);
    SetBuffer(stderr, NULL, bufferSize);
  }

  // lifemotif program exception
  catch(const LifeMotifException& e) {
    QString message;
    message = "Program is unexpectedly terminated due to the error:\n";
    message += QString(e.what());
    QMessageBox::critical(NULL, "Critical Error", message);
  }

  Fin();
  qDebug() << "Program finished.";
  return returnCode;
}

void Init()
{
  // this application's ini settings
  LifeMotifSettings::Init(LIFEMOTIF_DEFAULT_SETTINGS_PATH);

  if (Py_IsInitialized() == false) {
    Py_Initialize();
    qDebug() << "python initialized.";

    PythonWorkAround();
  }
}

void PythonWorkAround()
{
  const QString& qscriptpath = LifeMotifSettings::PythonScriptPath();
  std::string workAroundCode;

  workAroundCode += "import sys\n";
  workAroundCode += "sys.path.append('" + qscriptpath.toStdString() + "')\n";
  workAroundCode += "sys.path.append('.')\n";

  if (PyRun_SimpleString(workAroundCode.c_str()) != 0) {
    throw bp::error_already_set();
  }
  qDebug() << qscriptpath << "is now added to python path.";
}

void SetBuffer(FILE* stream, char* buffer, const int bufferSize)
{
  if (buffer) memset(buffer, 0, bufferSize);
  if(setvbuf(stream, buffer, _IOFBF, bufferSize)) {
    // failure.
    if (buffer) {
      free(buffer);
      buffer = NULL;
    }
    throw LifeMotifException("setvbuf failed!");
  }
}

void Fin()
{
  if (Py_IsInitialized() != 0) {
    Py_Finalize();
    qDebug() << "python finalized.";
  }
  qDebug() << "Finalization complete.";
}
