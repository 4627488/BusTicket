#include "Backend.h"
#include <fstream>
#include <qevent.h>
#include <QFile>
#include <qdebug.h>
#include <QCryptographicHash>
#include <chrono>
#include <iomanip>
#include <QDateTime>

const QString Backend::CONFIG_FILE = "bus.txt";
QString Backend::today_log = "log_" + QDateTime::currentDateTime().toString("yyyyMMdd") + ".txt";

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

QString Backend::hashPassword(const QString& password) {
	QCryptographicHash hash(QCryptographicHash::Sha256);
	hash.addData(password.toUtf8());
	return hash.result().toHex();
}

bool Backend::authenticate(const QString& username, const QString& password) {
	// TODO 从配置文件中读取用户名和密码而不是硬编码
	const QString storedUsername = "admin";
	//const QString storedPassword = "123";
	const QString storedHashedPassword = "a665a45920422f9d417e4867efdc4fb8a04a1f3fff1fa07e998e86f7f7a27ae3"; // 123
	return username == storedUsername && hashPassword(password) == storedHashedPassword;
}

Q_INVOKABLE QString Backend::add(const QString& trainNumber, const QString& departureTime, const QString& startStation, const QString& endStation, const QString& duration, const QString& price, const QString& capacity, const QString& soldTickets)
{
	try {
		//转换前检查
		if (trainNumber.isEmpty() || departureTime.isEmpty() || startStation.isEmpty() || endStation.isEmpty() || duration.isEmpty() || price.isEmpty() || capacity.isEmpty() || soldTickets.isEmpty())
			return "信息请填写完整";

		BusInfo bus{
			trainNumber,
			departureTime.split(":")[0].toInt(),
			departureTime.split(":")[1].toInt(),
			startStation,
			endStation,
			duration.toInt(),
			price.toInt(),
			capacity.toInt(),
			soldTickets.toInt()
		};
		return addBusInfo(bus);
	}
	catch (const std::exception& e) {
		// 处理异常
		qDebug() << "Caught exception: " << e.what();
		return "添加失败";
	}
}


bool Backend::loadBusInfo(const QString& filename) {
	timetables.clear();
	QFile file(filename);
	if (file.open(QIODevice::ReadOnly)) {
		QTextStream in(&file);
		while (!in.atEnd()) {
			BusInfo bus;
			in >> bus.busNumber >> bus.departureHour >> bus.departureMinute >> bus.startPoint >> bus.endPoint >> bus.duration >> bus.price >> bus.maxPassenger >> bus.soldTickets;
			if (bus.busNumber != "")
				timetables.append(bus);
		}
		file.close();
		return true;
	}
	return false;
}

QString Backend::addBusInfo(const BusInfo& bus) {
	// 确保车次号不重复
	for (const auto& b : timetables) {
		if (b.busNumber == bus.busNumber) {
			qWarning() << "Bus number already exists!";
			return "车次号已存在";
		}
	}
	timetables.append(bus);
	// 将信息写入配置文件中
	saveAllBusInfo(CONFIG_FILE);
	saveAllBusInfo(today_log);
	return "添加成功";
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

void Backend::saveAllBusInfo(const QString& filename) {
	QFile file(filename);
	if (file.open(QIODevice::WriteOnly)) { // 以只写的方式打开文件
		QTextStream out(&file);
		for (const auto& bus : timetables) {
			out << bus.busNumber << "\t" << bus.departureHour << "\t" << bus.departureMinute << "\t" << bus.startPoint << "\t" << bus.endPoint << "\t" << bus.duration << "\t" << bus.price << "\t" << bus.maxPassenger << "\t";
			if (filename == CONFIG_FILE)
				out << 0 << "\n";  // 保存到配置文件时，将售票数置为0
			else
				out << bus.soldTickets << "\n";
		}
		file.close();
	}
}

Q_INVOKABLE QString Backend::buyTicket(QString busNumber)
{
	for (auto& bus : timetables) {
		if (bus.busNumber == busNumber) {
			if (bus.soldTickets < bus.maxPassenger) {
				bus.soldTickets++;
				saveAllBusInfo(today_log);
				return "购票成功";
			}
			else {
				return "车票已售完";
			}
		}
	}
	return "没有找到该车次";
}

Q_INVOKABLE QString Backend::refundTicket(QString busNumber)
{
	for (auto& bus : timetables) {
		if (bus.busNumber == busNumber) {
			if (bus.soldTickets > 0) {
				bus.soldTickets--;
				saveAllBusInfo(today_log);
				return "退票成功";
			}
			else {
				return "没有车票可退";
			}
		}
	}
	return "没有找到该车次";
}

QString Backend::removeBusInfo(QString busNumberStr) {
	//int busNumber = busNumberStr.toInt();
	for (auto it = timetables.begin(); it != timetables.end(); ++it) {
		qDebug() << "busNumber: " << it->busNumber;
		if (it->busNumber == busNumberStr) {
			timetables.erase(it);
			// 将信息写入配置文件中
			saveAllBusInfo(CONFIG_FILE);
			saveAllBusInfo(today_log);
			return "删除成功";
		}
	}
	return "没有找到该车次";
}

void Backend::initSingleton()
{
	instance = new Backend();
	if (!QFile::exists(today_log))
		instance->loadBusInfo(CONFIG_FILE);
	else
		instance->loadBusInfo(today_log);
}
