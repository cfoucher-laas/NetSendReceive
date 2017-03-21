/*
 * Copyright © 2017 LAAS/CNRS
 * Author: Clément Foucher
 *
 * Distributed under the GNU GPL v2. For full terms see the file LICENSE.txt.
 *
 *
 * This file is part of Network Send / Receive (NSR).
 *
 * NSR is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 2 of the License.
 *
 * NSR is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with NSR. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef TABLEMODEL_H
#define TABLEMODEL_H

// Current class parent
#include <QAbstractTableModel>

// C++ classes
#include <memory>
using namespace std;


class TableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    enum class dataRepresentation { ASCII, Hexa };

public:
    explicit TableModel(shared_ptr<QByteArray> data, QObject* parent = nullptr);

    void setDataRepresentation(dataRepresentation representation);
    void setEditable(bool editable);
    void setTableData(shared_ptr<QByteArray> data);
    shared_ptr<QByteArray> getTableData();
	bool getEditable();

    virtual int columnCount(const QModelIndex& parent) const                              override;
    virtual int rowCount(const QModelIndex& parent) const                                 override;
    virtual QVariant data(const QModelIndex& index, int role) const                       override;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    virtual Qt::ItemFlags flags(const QModelIndex& index) const                           override;
    virtual bool setData(const QModelIndex& index, const QVariant& value, int role)       override;

private:
    void setTableDataInternal(shared_ptr<QByteArray> data);

private:
    shared_ptr<QByteArray> tableData;
    dataRepresentation representation;
    bool isEditable;
};

#endif // TABLEMODEL_H
