#include "ui.h"

#include <QtWidgets/QApplication>
#include <QtQml>
#include <qqml.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qmlRegisterType<Test>("com.mycompany.Test", 1, 0, "Test");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/ui.qml")));

    return a.exec();
}
