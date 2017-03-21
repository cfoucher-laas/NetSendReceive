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

#ifndef DATATABLEWIDGET_H
#define DATATABLEWIDGET_H

// Parent class
#include <QWidget>

// C++ classes
#include <memory>
using namespace std;

// Qt classes
class QPushButton;
class QRadioButton;
class QLabel;

// NSR classes
class DataTableView;


class DataTableWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DataTableWidget(bool enableLoad, bool enableSend, bool isEditable, QWidget* parent = nullptr);

    void retranslateUi();

    void setData(shared_ptr<QByteArray> data);

signals:
    void sendRequested(shared_ptr<QByteArray> data);

private slots:
    void setAsciiRepresentation(bool doSet);
    void setHexaRepresentation(bool doSet);

    void clearData();
    void removeLastByte();
    void saveData();
    void sendData();
    void loadData();

private:
    DataTableView* dataTableDisplay;

    QLabel*       labelRepresentation;
    QRadioButton* radioButtonHexa;
    QRadioButton* radioButtonAscii;

    QPushButton* buttonClear;
    QPushButton* buttonRemoveLast;
    QPushButton* buttonSave;
    // Optional buttons
    QPushButton* buttonSend;
    QPushButton* buttonLoad;
};

#endif // DATATABLEWIDGET_H
