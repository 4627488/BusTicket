#include "TableModel.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include "Backend.h"

TableModel::TableModel(QObject* parent)
    : QAbstractTableModel(parent)
{
    m_header = { "车次号", "发车时间", "始发站", "终到站", "大约时长", "票价", "最大载客量","已售票数"};
    // 判断 bus.txt 文件是否存在，如果存在则加载数据
    if (!QFile::exists(Backend::CONFIG_FILE)) {
        qDebug() << "File not found!";
    }
}

void TableModel::updateModel()
{
    beginResetModel();
	endResetModel();
}

int TableModel::rowCount(const QModelIndex& /*parent*/) const
{
    //qDebug() << m_data.size();
    return Backend::getInstance().getTimetables().size() + 1;
}

int TableModel::columnCount(const QModelIndex& /*parent*/) const
{
    //把时间小时和分钟合并成一个列
    return m_header.size();
}

QVariant TableModel::data(const QModelIndex& index, int role) const
{
    auto m_data = Backend::getInstance().getTimetables();
    if (role == Qt::DisplayRole) {
        if(index.row() == 0) {
            return m_header[index.column()];
		}
        auto &busItem=m_data[index.row()-1];
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
