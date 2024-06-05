#pragma once

#include <qqml.h>
#include <QAbstractTableModel>
#include <QStringList>
#include "Backend.h"
#include <mutex>

class TableModel : public QAbstractTableModel
{
	Q_OBJECT
		QML_ELEMENT
		QML_ADDED_IN_VERSION(1, 1)

public:
	TableModel(QObject* parent = nullptr);
	Q_INVOKABLE void updateModel();
	Q_INVOKABLE void setFilter(const bool isFiltered, const int FilterOption, const QString& filter);
	int rowCount(const QModelIndex & = QModelIndex()) const override;
	int columnCount(const QModelIndex & = QModelIndex()) const override;
	QVariant data(const QModelIndex& index, int role) const override;
	QHash<int, QByteArray> roleNames() const override;

private:
	QStringList m_header;
	QString Filter = "";
	int FilterOption = 0;
	QList<BusInfo> m_data;
	mutable std::mutex m_dataMutex;
	void updateData();
};
