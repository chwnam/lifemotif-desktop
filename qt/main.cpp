#include "mainwindow.h"
#include <QApplication>
#include <iostream>
#include <boost/python.hpp>
#include <Python.h>

int main(int argc, char *argv[])
{
//    QApplication a(argc, argv);
//    MainWindow w;
//    w.show();

//    return a.exec();
    Py_Initialize();
    PyRun_SimpleString(
                "import sys\n"
                "print sys.version\n");
    std::cout << "Hello!" << std::endl;
    Py_Finalize();
}
