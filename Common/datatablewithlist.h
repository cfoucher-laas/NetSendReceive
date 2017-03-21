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

#ifndef DATATABLEWITHLIST_H
#define DATATABLEWITHLIST_H

// Parent class
#include <QWidget>

// C++ classes
#include <memory>
using namespace std;

// Qt classes
class QListWidget;
class QPushButton;
class QLabel;

// NSR classes
class DataTableWidget;


class DataTableWithList : public QWidget
{
	Q_OBJECT

public:
	explicit DataTableWithList(QWidget* parent = nullptr);

	void retranslateUi();

	void addData(shared_ptr<QByteArray> newData, const QString& info);

private slots:
	void cleanList();
	void listRowSelectionChanged(int currentRow);

private:
	volatile bool suspendReception;

	QList<shared_ptr<QByteArray>> dataStorage;

	QLabel*      labelAvailableMessages;
	QListWidget* dataListDisplay;
	QPushButton* buttonClean;
	QLabel*      labelMessageContent;

	DataTableWidget* tableDisplay;
};

#endif // DATATABLEWITHLIST_H
