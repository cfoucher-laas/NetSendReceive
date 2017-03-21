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

// Current class header
#include "datatableview.h"

// Qt classes
#include <QHeaderView>


DataTableView::DataTableView(bool isEditable, QWidget* parent) :
    QTableView(parent)
{
    // Content
    this->tableModel = new TableModel(nullptr);

    if (isEditable == true)
    {
        this->tableModel->setEditable(true);
    }

    this->setModel(this->tableModel);

    // UI
    this->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

shared_ptr<QByteArray> DataTableView::getData()
{
    return this->tableModel->getTableData();
}

void DataTableView::setData(shared_ptr<QByteArray> data)
{
    this->tableModel->setTableData(data);
}

void DataTableView::clearData()
{
    this->tableModel->setTableData(nullptr);
}

void DataTableView::removeLastByte()
{
    shared_ptr<QByteArray> tableData = this->tableModel->getTableData();

    if (tableData->count() != 0)
    {
        tableData->resize(tableData->count()-1);
        this->tableModel->setTableData(tableData);
    }
}

void DataTableView::setDataRepresentation(TableModel::dataRepresentation representation)
{
	this->tableModel->setDataRepresentation(representation);
}

bool DataTableView::isEditable()
{
	return this->tableModel->getEditable();
}
