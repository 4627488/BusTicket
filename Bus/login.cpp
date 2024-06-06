#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickItem>
#include <QQuickWindow>
#include <QQmlContext>
#include "Backend.h"
#include "TableModel.h"
#include <QFile>

void messageHandler(QtMsgType type, const QMessageLogContext& context, const QString& msg) {
	QFile file("log.txt");
	file.open(QIODevice::Append | QIODevice::Text);
	QTextStream out(&file);
	out << msg << "\n";
	file.close();
}


int main(int argc, char* argv[])
{
#if defined(Q_OS_WIN)
	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

	QGuiApplication app(argc, argv);

	qmlRegisterType<TableModel>("TableModel", 1, 0, "TableModel");
	qInstallMessageHandler(messageHandler);

	QQmlApplicationEngine engine;

	engine.rootContext()->setContextProperty("backend", &Backend::getInstance());

	engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
	//engine.load(QUrl(QStringLiteral("qrc:/ControlPanel.qml")));
	if (engine.rootObjects().isEmpty())
		return -1;

	return app.exec();
}
