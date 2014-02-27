#include <QApplication>
#include "main_window.h"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    MainWindow   win;

    win.show();
    win.raise();
    win.activateWindow();

    return app.exec();
}
