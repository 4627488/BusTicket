#include "TableModel.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include "Backend.h"
#include <mutex>

TableModel::TableModel(QObject* parent)
	: QAbstractTableModel(parent)
{
	m_header = { "车次号", "发车时间", "始发站", "终到站", "大约时长", "票价", "最大载客量","已售票数" };
	updateData();
}

void TableModel::updateModel()
{
	updateData();
	beginResetModel();
	endResetModel();
}

Q_INVOKABLE void TableModel::setFilter(const bool isFiltered, const int FilterOption, const QString& filter)
{
	Filter = filter;
	this->FilterOption = FilterOption;
	if (!isFiltered) {
		this->Filter = "";
	}
	updateModel();
}

int TableModel::rowCount(const QModelIndex& /*parent*/) const
{
	//qDebug() << m_data.size();
	std::lock_guard<std::mutex> lock(m_dataMutex);
	return m_data.size() + 1;
}

int TableModel::columnCount(const QModelIndex& /*parent*/) const
{
	//把时间小时和分钟合并成一个列
	return m_header.size();
}

QVariant TableModel::data(const QModelIndex& index, int role) const
{
	if (role == Qt::DisplayRole) {
		if (index.row() == 0) {
			return m_header[index.column()];
		}
		std::lock_guard<std::mutex> lock(m_dataMutex);
		auto& busItem = m_data[index.row() - 1];
		switch (index.column())
		{
		case 0:
			return busItem.busNumber;
		case 1:
			return QString::number(busItem.departureHour) + ":" + QString::number(busItem.departureMinute);
		case 2:
			return busItem.startPoint;
		case 3:
			return busItem.endPoint;
		case 4:
			return busItem.duration;
		case 5:
			return busItem.price;
		case 6:
			return busItem.maxPassenger;
		case 7:
			return busItem.soldTickets;
		default:
			break;
		}
	}
	return QVariant();
}

QHash<int, QByteArray> TableModel::roleNames() const
{
	return { {Qt::DisplayRole, "display"} };
}

void TableModel::updateData()
{
	auto all_data = Backend::getInstance().getTimetables();

	// 使用互斥锁保护 m_data
	std::lock_guard<std::mutex> lock(m_dataMutex);

	//过滤
	m_data.clear();
	for (auto& bus : all_data) {
		if (Filter.isEmpty()) {
			m_data.append(bus);
			continue;
		}
		if (FilterOption == 0 && bus.busNumber == Filter.toInt()) {
			m_data.append(bus);
		}
		else if (FilterOption == 1 && bus.startPoint == Filter) {
			m_data.append(bus);
		}
		else if (FilterOption == 2 && bus.endPoint == Filter) {
			m_data.append(bus);
		}
	}
	std::sort(m_data.begin(), m_data.end(), [](const BusInfo& a, const BusInfo& b) {
		return a.departureHour * 60 + a.departureMinute < b.departureHour * 60 + b.departureMinute;
		});
}
