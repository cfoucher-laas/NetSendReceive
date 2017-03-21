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
#include "tablemodel.h"

#include <QColor>


TableModel::TableModel(shared_ptr<QByteArray> data, QObject* parent) :
    QAbstractTableModel(parent)
{
    this->isEditable     = false;
    this->representation = dataRepresentation::Hexa;

    this->setTableDataInternal(data);
}

void TableModel::setDataRepresentation(TableModel::dataRepresentation representation)
{
    emit this->beginResetModel();

    this->representation = representation;

    emit this->endResetModel();
}

void TableModel::setEditable(bool editable)
{
    emit this->beginResetModel();

    this->isEditable = editable;

    if ( (this->isEditable == true) && (this->tableData == nullptr) )
    {
        // Make sure data exists if table is editable
        this->setTableDataInternal(nullptr);
    }

    emit this->endResetModel();
}

void TableModel::setTableData(shared_ptr<QByteArray> data)
{
    emit this->beginResetModel();

    this->setTableDataInternal(data);

    emit this->endResetModel();
}

shared_ptr<QByteArray> TableModel::getTableData()
{
	return this->tableData;
}

bool TableModel::getEditable()
{
	return this->isEditable;
}

int TableModel::columnCount(const QModelIndex&) const
{
    return 8;
}

int TableModel::rowCount(const QModelIndex&) const
{
    int value = 0;

    if (this->tableData != nullptr)
    {
        value = this->tableData->count() / 8;

        if ((this->tableData->count() % 8) != 0)
        {
            // For incomplete lines
            value++;
        }
        else
        {
            if (this->isEditable)
            {
                // Allow an empty line if data is editable
                value++;
            }
        }
    }

    return value;
}

QVariant TableModel::data(const QModelIndex& index, int role) const
{
    QVariant variant = QVariant();

    if (index.isValid())
    {
        if ((role == Qt::DisplayRole) || (role == Qt::EditRole))
        {
            if (this->tableData != nullptr)
            {
                int offset = index.row()*8 + index.column();

                if (offset < this->tableData->count())
                {
                    unsigned char value = (unsigned char)this->tableData->at(offset);

                    if (this->representation == dataRepresentation::Hexa)
                    {
                        QString text = QString::number(value, 16);
                        text = text.toUpper();
                        if (text.size() == 1)
                        {
                            text = "0" + text;
                        }

                        variant = text;
                    }
                    else if (this->representation == dataRepresentation::ASCII)
                    {
                        if ( (value >= 32) && (value != 127) )
                        {
                            variant = QString(QChar(this->tableData->at(offset)));
                        }
                        else
                        {
                            variant = "[?]";
                        }

                    }
                }
            }
        }
        else if (role == Qt::BackgroundColorRole)
        {
            if ( (index.row() % 2) == 0)
                variant = QVariant(QColor(0, 0, 255, 25));
            else
                variant = QVariant(QColor(0, 0, 255, 50));
        }
    }

    return variant;
}

QVariant TableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    QVariant variant = QVariant();

    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal)
        {
            variant = "+ " + QString::number(section);
        }
        else
        {
            variant = QVariant(section*8);
        }
    }

    return variant;
}

Qt::ItemFlags TableModel::flags(const QModelIndex& index) const
{
    Qt::ItemFlags flags = Qt::NoItemFlags;

    if (index.isValid())
    {
        flags = Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemNeverHasChildren;

        if (this->isEditable == true)
        {
            flags |= Qt::ItemIsEditable;
        }
    }

    return flags;
}

bool TableModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    bool dataValid = false;
    char newValue;
    bool renewTab = false;

    if (role == Qt::EditRole)
    {
        if (this->representation == dataRepresentation::Hexa)
        {
            QString stringValue = value.toString();

            if (stringValue.size() <= 2)
            {
                bool ok;
                newValue = (char)stringValue.toInt(&ok, 16);

                if (ok == true)
                {
                    dataValid = true;
                }
            }
        }
        else if (this->representation == dataRepresentation::ASCII)
        {
            QString stringValue = value.toString();

            if (stringValue.size() == 1)
            {
                newValue = stringValue[0].toLatin1();
                dataValid = true;
            }
        }

        if (dataValid == true)
        {
            int offset = index.row()*8 + index.column();

            int previousDataCount = this->tableData->count();
            if (offset >= previousDataCount)
            {
                // Check if data is being inserted
                emit this->beginResetModel();
                this->tableData->resize(offset+1);
                for (int i = previousDataCount; i < offset ; i++)
                {
                    *(this->tableData->data() + i) = 0;
                }

                renewTab = true;
            }

            *(this->tableData->data() + offset) = newValue;
        }
    }

    if (renewTab == true)
    {
        emit this->endResetModel();
    }

    return dataValid;
}

void TableModel::setTableDataInternal(shared_ptr<QByteArray> data)
{
    if (this->isEditable == false)
    {
        this->tableData = data;
    }
    else
    {
        if (data != nullptr)
        {
            this->tableData = data;
        }
        else
        {
            this->tableData = shared_ptr<QByteArray>(new QByteArray());
        }
    }
}
