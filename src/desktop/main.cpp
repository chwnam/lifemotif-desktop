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
#include "lifemotif_google_client_info.h"

void Init(QApplication* app);
void Fin();
void SetBuffer(FILE* stream, char* buffer, const int bufferSize);

int main(int argc, char *argv[])
{
  int returnCode = EXIT_FAILURE;
  QApplication app(argc, argv);

  try {
    Init(&app);

    MainWindow w;
    w.show();
    w.raise();
    w.activateWindow();

    returnCode = app.exec();
  }

  // lifemotif program exception
  catch(const LifeMotifException& e) {
    QString message;
    message = "Program is unexpectedly terminated due to the error:\n";
    message += QString(e.what());
    QMessageBox::critical(NULL, "Critical Error", message);
  }

  Fin();
  return returnCode;
}

void Init(QApplication* app)
{
  // this application's ini settings
  LifeMotif::Settings::Init(app);
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
  qDebug() << "Finalization complete.";
}
