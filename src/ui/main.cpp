#include "pictureprocessorui.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PictureProcessorUI w;
    w.show();
    return a.exec();
}
