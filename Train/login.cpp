#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickItem>
#include <QQuickWindow>
#include <QQmlContext>
#include "Backend.h"
#include "TableModel.h"

int main(int argc, char *argv[])
{
#if defined(Q_OS_WIN)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);

    qmlRegisterType<TableModel>("TableModel", 1, 0, "TableModel");

    QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty("backend", &Backend::getInstance());

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    //engine.load(QUrl(QStringLiteral("qrc:/AdminControlPanel.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
