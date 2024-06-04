#pragma once
#include <QObject>
#include <QString>
#include <QVector>

struct BusInfo {
	int busNumber;
	int departureHour;
	int departureMinute;
	QString startPoint;
	QString endPoint;
	int duration;
	int price;
	int maxPassenger;
	int soldTickets;
};

class Backend : public QObject
{
	Q_OBJECT

public:
	static Backend& getInstance(); // 单例模式
	const static QString CONFIG_FILE;
	QList<BusInfo> getTimetables();
	Q_INVOKABLE bool authenticate(const QString& username, const QString& password);
	//tableModel.add(trainNumberField.text, departureTimeField.text, startStationField.text, endStationField.text, durationField.text, priceField.text, capacityField.text, soldTicketsField.text)
	Q_INVOKABLE QString add(const QString& trainNumber, const QString& departureTime, const QString& startStation, const QString& endStation, const QString& duration, const QString& price, const QString& capacity, const QString& soldTickets);
	void loadBusInfo(const QString& filename);
	void addBusInfo(const BusInfo& bus);
	void displayAllBusInfo();
	void saveAllBusInfo();
	Q_INVOKABLE QString removeBusInfo(QString busNumber);


private:
	Backend() {}  // 私有化构造函数
	Backend(const Backend&) = delete;  // 禁止复制
	Backend& operator=(const Backend&) = delete;  // 禁止赋值
	static Backend* instance;  // 静态成员变量
	static std::once_flag initInstanceFlag;
	static void initSingleton();

	QVector<BusInfo> timetables;
};



