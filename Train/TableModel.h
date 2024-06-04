#pragma once

#include <qqml.h>
#include <QAbstractTableModel>
#include <QStringList>
#include "Backend.h"

class TableModel : public QAbstractTableModel
{
    Q_OBJECT
    QML_ELEMENT
    QML_ADDED_IN_VERSION(1, 1)

public:
    TableModel(QObject* parent = nullptr);
    Q_INVOKABLE void updateModel();
    int rowCount(const QModelIndex & = QModelIndex()) const override;
    int columnCount(const QModelIndex & = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    //Q_INVOKABLE QString getHeaderData(int section, Qt::Orientation orientation) const;

    //QVariant headerData(int section, Qt::Orientation orientation, int role) const;

private:
    QStringList m_header;
};
