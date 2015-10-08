#include <QtWidgets/QApplication>
#include <QtQml>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/ui.qml")));

    return a.exec();
}
