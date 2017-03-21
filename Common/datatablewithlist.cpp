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
#include "datatablewithlist.h"

// Qt classes
#include <QGridLayout>
#include <QPushButton>
#include <QListWidget>
#include <QLabel>

// NSR classes
#include "datatablewidget.h"


DataTableWithList::DataTableWithList(QWidget* parent) :
	QWidget(parent)
{
	this->suspendReception = false;

	QGridLayout* gridLayout = new QGridLayout(this);
	gridLayout->setSpacing(10);
	gridLayout->setContentsMargins(0, 0, 0, 0);

	this->labelAvailableMessages = new QLabel(this);
	gridLayout->addWidget(this->labelAvailableMessages, 0, 0, 1, -1, Qt::AlignCenter);

	this->dataListDisplay = new QListWidget(this);
	gridLayout->addWidget(this->dataListDisplay, 1, 0, 1, 1);

	this->buttonClean = new QPushButton(this);
	gridLayout->addWidget(this->buttonClean, 1, 1, 1, 1);

	this->labelMessageContent = new QLabel(this);
	gridLayout->addWidget(this->labelMessageContent, 2, 0, 1, -1, Qt::AlignCenter);

	this->tableDisplay = new DataTableWidget(false, false, false, this);
	gridLayout->addWidget(this->tableDisplay, 3, 0, 1, -1);

	this->retranslateUi();

	connect(this->dataListDisplay, &QListWidget::currentRowChanged, this, &DataTableWithList::listRowSelectionChanged);
	connect(this->buttonClean,     &QPushButton::clicked,           this, &DataTableWithList::cleanList);
}

void DataTableWithList::retranslateUi()
{
	this->buttonClean->setText(tr("Clean list"));

	this->labelAvailableMessages->setText("<b>" + tr("Received messages") + " </b>");
	this->labelMessageContent->setText("<b>" + tr("Selected message content") + "</b>");

	this->tableDisplay->retranslateUi();
}

void DataTableWithList::addData(shared_ptr<QByteArray> newData, const QString& info)
{
	while (this->suspendReception == true)
	{
		// Wait
	}

	this->dataStorage.append(newData);

	QString messageText = tr("Message") + " #" + QString::number(this->dataListDisplay->count());
	if (info.isEmpty() == false)
	{
		messageText += " (" + info + ")";
	}

	this->dataListDisplay->addItem(messageText);
}

void DataTableWithList::cleanList()
{
	this->suspendReception = true;

	this->dataStorage.clear();
	this->dataListDisplay->clear();
	this->tableDisplay->setData(nullptr);

	this->suspendReception = false;
}

void DataTableWithList::listRowSelectionChanged(int currentRow)
{
	if (currentRow != -1)
	{
		this->tableDisplay->setData(this->dataStorage[currentRow]);
	}
}
