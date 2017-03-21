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

#ifndef DATATABLEVIEW_H
#define DATATABLEVIEW_H

// Parent class
#include <QTableView>

// C++ classes
#include <memory>
using namespace std;

// NSR classes
#include "tablemodel.h"


class DataTableView : public QTableView
{
public:
    explicit DataTableView(bool isEditable, QWidget* parent = nullptr);

public:
    shared_ptr<QByteArray> getData();
    void setData(shared_ptr<QByteArray> data);
    void clearData();
    void removeLastByte();
    void setDataRepresentation(TableModel::dataRepresentation representation);

	bool isEditable();

private:
    TableModel* tableModel;
};

#endif // DATATABLEVIEW_H
