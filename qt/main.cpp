#include "mainwindow.h"
#include <QApplication>
#include <QDir>
#include <iostream>
#include "./python_wrapper/googleoauth2wrapper.h"
#include "./python_wrapper/googleimapwrapper.h"
#include "./python_wrapper/localstructurewrapper.h"
#include "settingscontrol.h"

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

void testOauth2WrapperAuthentication()
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

void CloseImapObject(bp::object& imapObject)
{
  PyObject_CallMethod(imapObject.ptr(), "close", NULL);
  PyObject_CallMethod(imapObject.ptr(), "logout", NULL);
}

void testImap()
{
  std::string oauth2ModuleName = "google_oauth2_control";
  std::string oauth2ClassName = "google_oauth2_control";

  std::string imapModuleName = "google_imap_control";
  std::string imapClassName = "google_imap_control";

  std::string structureModuleName = "local_structure_control";
  std::string structureClassName = "local_structure_control";

  std::string storageName = GetPythonPath() + "credential.json";
  std::string label = "개인/일기";

  GoogleOauth2Wrapper oauth2Wrapper(oauth2ModuleName, oauth2ClassName);
  bp::object imapObject;

  imapObject = oauth2Wrapper.ImapAuthenticate(
        storageName, "cs.chwnam@gmail.com", 4);

  GoogleImapWrapper imapWrapper(imapModuleName, imapClassName, imapObject);
  bp::object structure;

  //imapWrapper.ListMailBox();
  structure = imapWrapper.FetchThreadStructure(label);

  LocalStructureWrapper
      structureWrapper(structureModuleName, structureClassName);

  structureWrapper.Build(GetPythonPath() + "build_by_cpp.dat", structure);

  CloseImapObject(imapObject);
}

void testJson()
{
  std::string python = GetPythonPath() + "config.json";
  std::string path = GetPythonPath();

  SettingsControl::Init(QDir::currentPath() + "/config.ini");
  QSettings& settings = SettingsControl::GetSettings();

  //settings.setValue("python_config", QString::fromStdString(python));
  QString python_config = settings.value("python_config").toString();

  std::cout << python_config.toStdString() << std::endl;
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
    //testOauth2WrapperAuthentication();
    //testImap();
    testJson();
  } catch (const bp::error_already_set &) {
    PyErr_Print();
  }

  Py_Finalize();

  return EXIT_SUCCESS;
}
