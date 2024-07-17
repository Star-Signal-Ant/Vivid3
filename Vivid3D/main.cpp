#include "Vivid3D.h"
#include <QtWidgets/QApplication>
#include "GeneralInput.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    Vivid3D w;
    w.show();
    return a.exec();
}
