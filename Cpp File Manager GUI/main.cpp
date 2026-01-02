#include "CppFileManagerGUI.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    CppFileManagerGUI window;
    window.show();
    return app.exec();
}
