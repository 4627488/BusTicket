#include "Backend.h"
#include <fstream>
#include <qevent.h>
#include <QFile>
#include <qdebug.h>

const QString Backend::CONFIG_FILE = "bus.txt";
// 在类外初始化静态成员
Backend* Backend::instance = nullptr;
std::once_flag Backend::initInstanceFlag;

// 单例模式：确保一个类只有一个实例，并提供一个全局访问点
Backend& Backend::getInstance()
{
	std::call_once(initInstanceFlag, &Backend::initSingleton); // 保证线程安全，只调用一次initSingleton
	return *instance;
}

QList<BusInfo> Backend::getTimetables() {
	if (timetables.isEmpty()) {
		loadBusInfo(CONFIG_FILE);
	}
	return timetables;
}

bool Backend::authenticate(const QString& username, const QString& password) {
	// TODO 从配置文件中读取用户名和密码而不是硬编码
	const QString storedUsername = "admin";
	const QString storedPassword = "123";
	return username == storedUsername && password == storedPassword;
}

Q_INVOKABLE QString Backend::add(const QString& trainNumber, const QString& departureTime, const QString& startStation, const QString& endStation, const QString& duration, const QString& price, const QString& capacity, const QString& soldTickets)
{
	try {
		//转换前检查
		BusInfo bus{
			trainNumber.toInt(),
			departureTime.split(":")[0].toInt(),
			departureTime.split(":")[1].toInt(),
			startStation,
			endStation,
			duration.toInt(),
			price.toInt(),
			capacity.toInt(),
			soldTickets.toInt()
		};
		addBusInfo(bus);
		return "ok";
	}
	catch (const std::exception& e) {
		// 处理异常
		qDebug() << "Caught exception: " << e.what();
		return "error";
	}
}


void Backend::loadBusInfo(const QString& filename) {
	timetables.clear();
	QFile file(filename);
	if (file.open(QIODevice::ReadOnly)) {
		QTextStream in(&file);
		while (!in.atEnd()) {
			BusInfo bus;
			in >> bus.busNumber >> bus.departureHour >> bus.departureMinute >> bus.startPoint >> bus.endPoint >> bus.duration >> bus.price >> bus.maxPassenger >> bus.soldTickets;
			if (bus.busNumber != 0)
				timetables.append(bus);
		}
		file.close();
	}
}

void Backend::addBusInfo(const BusInfo& bus) {
	// 确保车次号不重复
	for (const auto& b : timetables) {
		if (b.busNumber == bus.busNumber) {
			qWarning() << "Bus number already exists!";
			return;
		}
	}
	timetables.append(bus);
	// 将信息写入配置文件中
	saveAllBusInfo();
}

void Backend::displayAllBusInfo() {
	for (const auto& bus : timetables) {
		qWarning() << "Bus number: " << bus.busNumber;
		qWarning() << "Departure time: " << bus.departureHour << ":" << bus.departureMinute;
		qWarning() << "Start point: " << bus.startPoint;
		qWarning() << "End point: " << bus.endPoint;
		qWarning() << "Duration: " << bus.duration << " minutes";
		qWarning() << "Price: " << bus.price;
		qWarning() << "Max passenger: " << bus.maxPassenger;
		qWarning() << "Sold tickets: " << bus.soldTickets;
		qWarning() << "----------------------";
	}
}

void Backend::saveAllBusInfo() {
	QFile file(CONFIG_FILE);
	if (file.open(QIODevice::WriteOnly)) { // 以只写的方式打开文件
		QTextStream out(&file);
		for (const auto& bus : timetables) {
			out << bus.busNumber << "\t" << bus.departureHour << "\t" << bus.departureMinute << "\t" << bus.startPoint << "\t" << bus.endPoint << "\t" << bus.duration << "\t" << bus.price << "\t" << bus.maxPassenger << "\t" << bus.soldTickets << "\n";
		}
		file.close();
	}
}

QString Backend::removeBusInfo(QString busNumberStr) {
	int busNumber = busNumberStr.toInt();
	for (auto it = timetables.begin(); it != timetables.end(); ++it) {
		qDebug() << "busNumber: " << it->busNumber;
		if (it->busNumber == busNumber) {
			timetables.erase(it);
			// 将信息写入配置文件中
			saveAllBusInfo();
			return "ok";
		}
	}
	return "Bus number not found!";
}

void Backend::initSingleton()
{
	instance = new Backend();
	instance->loadBusInfo(CONFIG_FILE);
}
