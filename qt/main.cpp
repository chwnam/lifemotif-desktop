#include "mainwindow.h"
#include <QApplication>
#include <iostream>
#include "./python_wrapper/googleoauth2wrapper.h"

std::string GetPythonPath()
{
  std::string root = "/Users/changwoo/Developer/lifemotif-desktop/";
  std::string path = root + "python/";

  return path;
}

void workaround()
{
  // workaround
  std::string path = GetPythonPath();
  std::string runCode;

  runCode += "import sys\n";
  runCode += "sys.path.append('" + path + "')\n";
  runCode += "sys.path.append('.')\n";

  PyRun_SimpleString(runCode.c_str());
}

void testOauth2Wrapper()
{
  std::string moduleName = "google_oauth2_control";
  std::string className = "google_oauth2_control";

  std::string secretPath = GetPythonPath() + "client_secrets.json";
  std::string storageName = GetPythonPath() + "credential.json";

  GoogleOauth2Wrapper wrapper(moduleName, className);
  std::string url;
  std::string code;
  bp::object credentials;

  url = wrapper.GetAuthorizationURL(secretPath);
  code = wrapper.GrantUserPermission(url);
  credentials = wrapper.MakeCredentials(code);
  wrapper.SetCredentials(storageName, credentials);
}


int main(int argc, char *argv[])
{
  //    QApplication a(argc, argv);
  //    MainWindow w;
  //    w.show();

  //    return a.exec();
  Py_Initialize();

  try {
    workaround();
    testOauth2Wrapper();
  } catch (const bp::error_already_set &) {
    PyErr_Print();
  }

  Py_Finalize();

  return EXIT_SUCCESS;
}
