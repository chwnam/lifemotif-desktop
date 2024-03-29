#include "main_window.h"
#include <QApplication>
#include <QDebug>
#include <QMessageBox>
#include <cstdio>
#include <string>

#include "config.h"
#include "exceptions.h"
#include "settings.h"


void Init();
void Fin();
void SetBuffer(FILE* stream, char* buffer, const int bufferSize);

int main(int argc, char *argv[])
{
  int returnCode = EXIT_FAILURE;
  QApplication app(argc, argv);

  try {
    Init();

    MainWindow w;
    w.show();
    w.raise();
    w.activateWindow();

    returnCode = app.exec();
  }

  // lifemotif program exception
  catch(const LifeMotif::Exception& e) {
    QString message;
    message = "Program is unexpectedly terminated due to the error:\n";
    message += QString(e.what());
    QMessageBox::critical(NULL, "Critical Error", message);
  }

  Fin();
  return returnCode;
}

void Init()
{
  QCoreApplication::setOrganizationName(Config::organizationName);
  QCoreApplication::setOrganizationDomain(Config::organizationDomain);
  QCoreApplication::setApplicationName(Config::applicationName);
  QCoreApplication::setApplicationVersion(QString::fromUtf8(Config::Version()));
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
    throw LifeMotif::Exception("setvbuf failed!");
  }
}

void Fin()
{
  qDebug() << "Finalization complete.";
}
